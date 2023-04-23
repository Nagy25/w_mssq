#ifndef FLUTTER_PLUGIN_W_MSSQ_PLUGIN_H_
#define FLUTTER_PLUGIN_W_MSSQ_PLUGIN_H_

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>

#include <memory>

namespace w_mssq {

class WMssqPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  WMssqPlugin();

  virtual ~WMssqPlugin();

  // Disallow copy and assign.
  WMssqPlugin(const WMssqPlugin&) = delete;
  WMssqPlugin& operator=(const WMssqPlugin&) = delete;

 private:
  // Called when a method is called on this plugin's channel from Dart.
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
};

}  // namespace w_mssq

#endif  // FLUTTER_PLUGIN_W_MSSQ_PLUGIN_H_
