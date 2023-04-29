#include "w_mssq_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>
#include <sql.h>
#include <sqlext.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <memory>
#include <sstream>
#include <iostream>
#include <wchar.h>
#include <vector>
#include <map>
#include <locale>
#include <codecvt>
#include <string>


typedef std::map<std::string, std::string> Row;
typedef std::vector<Row> ResultSet;

SQLHDBC dbc;
SQLHENV env;



namespace w_mssq {
std::string getColumnName(SQLHSTMT stmt, int col);
std::string wstring_to_utf8(const std::wstring& wstr);

std::string sqlConnection(const std::string& serverName) {
    typedef __int64 INT64;

    SQLRETURN ret;

    // Allocate environment handle
    ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        return "Error allocating environment handle";
    }

    // Set the ODBC version to use
    ret = SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        SQLFreeHandle(SQL_HANDLE_ENV, env);
        return "Error setting ODBC version";
    }

    // Allocate connection handle
    ret = SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {

        SQLFreeHandle(SQL_HANDLE_ENV, env);
        return "Error allocating connection handle";
    }

    // Connect to SQL Server using Windows authentication
    std::wstring wideServerName(serverName.begin(), serverName.end());
    std::wstring connectionStr = L"DRIVER={SQL Server};SERVER=" + wideServerName + L";Trusted_Connection=yes;";
    ret = SQLDriverConnect(dbc, NULL, (SQLTCHAR*)connectionStr.c_str(), SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);

    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {


        // Get the error message
        SQLWCHAR sqlState[6], message[SQL_MAX_MESSAGE_LENGTH];
        SQLINTEGER nativeError;
        SQLSMALLINT i = 1, length = 0;
        while (SQLGetDiagRec(SQL_HANDLE_DBC, dbc, i, sqlState, &nativeError, message, SQL_MAX_MESSAGE_LENGTH, &length) != SQL_NO_DATA) {
            std::wcout << "Error " << i << ": " << message << std::endl;
            i++;
        }

        SQLFreeHandle(SQL_HANDLE_DBC, dbc);
        SQLFreeHandle(SQL_HANDLE_ENV, env);
        return "Error connecting to SQL Server";
    }

    return "Connected to SQL Server";



}


std::vector<std::map<std::string, std::string>> sqlQuery(const std::wstring& query) {
    SQLRETURN ret;
    SQLHSTMT stmt;
    SQLSMALLINT columns;
    SQLLEN rows;
    SQLWCHAR buffer[SQL_MAX_MESSAGE_LENGTH];
    std::vector<std::map<std::string, std::string>> result;

    // Allocate statement handle
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        result.push_back({ {"Error", "allocating statement handle"} });
        return result;
    }

    // Execute SQL query
    ret = SQLExecDirect(stmt, (SQLWCHAR*)query.c_str(), SQL_NTS);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        result.push_back({ {"Error", "executing SQL query"} });
        return result;
    }

    // Get number of columns in result set
    ret = SQLNumResultCols(stmt, &columns);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        result.push_back({ {"Error", "getting number of result columns"} });
        return result;
    }

    // Fetch and add each row of data to result vector
    while (SQLFetch(stmt) == SQL_SUCCESS) {
        std::map<std::string, std::string> row;
        for (int i = 1; i <= columns; i++) {
            ret = SQLGetData(stmt, static_cast<SQLUSMALLINT>(i), SQL_C_WCHAR, buffer, SQL_MAX_MESSAGE_LENGTH, &rows);
            if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
                SQLFreeHandle(SQL_HANDLE_STMT, stmt);
                result.push_back({ {"Error", "fetching data"} });
                return result;
            }

      row[getColumnName(stmt,i)] = std::string(static_cast<const char*>(reinterpret_cast<const void*>(buffer)), static_cast<std::size_t>(wcslen(buffer) * sizeof(wchar_t)));


        }
        result.push_back(row);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    return result;
}



std::string getColumnName(SQLHSTMT stmt, int col) {
    SQLWCHAR col_name[256];
    SQLSMALLINT col_name_len;
    SQLRETURN ret = SQLDescribeCol(stmt, (SQLUSMALLINT)col, col_name, 256, &col_name_len, NULL, NULL, NULL, NULL);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        return "Error getting column name";
    }
   std::wstring wstr(col_name, col_name + col_name_len);
       std::string str =  wstring_to_utf8(wstr);
    return str;
}

std::string wstring_to_utf8(const std::wstring& wstr) {
    int utf8_length = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    if (utf8_length == 0) {
        return "";
    }
    std::string str(utf8_length - 1, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], utf8_length, NULL, NULL);
    return str;
}





void closeConnection() {
    // Cleanup
    SQLDisconnect(dbc);
    SQLFreeHandle(SQL_HANDLE_DBC, dbc);
    SQLFreeHandle(SQL_HANDLE_ENV, env);
}



// static
void WMssqPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows *registrar) {
  auto channel =
      std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
          registrar->messenger(), "w_mssq",
          &flutter::StandardMethodCodec::GetInstance());

  auto plugin = std::make_unique<WMssqPlugin>();

  channel->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto &call, auto result) {
        plugin_pointer->HandleMethodCall(call, std::move(result));
      });

  registrar->AddPlugin(std::move(plugin));
}

WMssqPlugin::WMssqPlugin() {}

WMssqPlugin::~WMssqPlugin() {}




void WMssqPlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue>& method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
 if (method_call.method_name().compare("sqlConnection") == 0) {
    // code for returnString function
    std::string serverName;
    std::string r;
    if (auto arguments = std::get_if<flutter::EncodableMap>(method_call.arguments())) {
      if (arguments->find(flutter::EncodableValue("serverName")) != arguments->end()) {
        serverName = std::get<std::string>(arguments->at(flutter::EncodableValue("serverName")));
           r = sqlConnection(serverName);

      }
    }
    result->Success(flutter::EncodableValue(r));
  }else if(method_call.method_name().compare("closeConnection") == 0){
  closeConnection();
  } else {
    result->NotImplemented();
  }
}




}
 // namespace w_mssq
