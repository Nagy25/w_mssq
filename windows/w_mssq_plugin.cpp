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

namespace w_mssq {

std::string sqlConnect(const std::string& input) {
    typedef __int64 INT64;

    SQLHENV env;
    SQLHDBC dbc;
    SQLRETURN ret;
 // Allocate environment handle
     ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
     if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
         std::cout << "Error allocating environment handle" << std::endl;
         return "Error allocating environment handle";
     }

     // Set the ODBC version to use
     ret = SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
     if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
         std::cout << "Error setting ODBC version" << std::endl;
         SQLFreeHandle(SQL_HANDLE_ENV, env);
         return "Error setting ODBC version";
     }

     // Allocate connection handle
     ret = SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);
     if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
         std::cout << "Error allocating connection handle" << std::endl;
         SQLFreeHandle(SQL_HANDLE_ENV, env);
         return "Error allocating connection handle";
     }

     // Connect to SQL Server using Windows authentication
     ret = SQLDriverConnect(dbc, NULL, (SQLTCHAR*)L"DRIVER={SQL Server};SERVER=AHMED-NAGY\\MSSQLSERVER01;Trusted_Connection=yes;", SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);
     if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
         std::cout << "Error connecting to SQL Server" << std::endl;

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

     // Connection successful, print message
     std::cout << "Connected to SQL Server!" << std::endl;

     // Cleanup
     SQLDisconnect(dbc);
     SQLFreeHandle(SQL_HANDLE_DBC, dbc);
     SQLFreeHandle(SQL_HANDLE_ENV, env);

     return "connected";
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
 if (method_call.method_name().compare("sqlConnect") == 0) {
    // code for returnString function
    std::string input;
    std::string r;
    if (auto arguments = std::get_if<flutter::EncodableMap>(method_call.arguments())) {
      if (arguments->find(flutter::EncodableValue("input")) != arguments->end()) {
        input = std::get<std::string>(arguments->at(flutter::EncodableValue("input")));
           r = sqlConnect("test");

      }
    }
    result->Success(flutter::EncodableValue(r));
  } else {
    result->NotImplemented();
  }
}




}
 // namespace w_mssq
