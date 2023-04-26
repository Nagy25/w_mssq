#include "w_mssq_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <memory>
#include <sstream>

namespace w_mssq {

std::string ReturnString(const std::string& input) {
  return input;
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
  if (method_call.method_name().compare("getPlatformVersion") == 0) {
    // code for getPlatformVersion function
  } else if (method_call.method_name().compare("returnString") == 0) {
    // code for returnString function
    std::string input;
    if (auto arguments = std::get_if<flutter::EncodableMap>(method_call.arguments())) {
      if (arguments->find(flutter::EncodableValue("input")) != arguments->end()) {
        input = std::get<std::string>(arguments->at(flutter::EncodableValue("input")));
      }
    }
    result->Success(flutter::EncodableValue(input));
  } else {
    result->NotImplemented();
  }
}




}
 // namespace w_mssq
