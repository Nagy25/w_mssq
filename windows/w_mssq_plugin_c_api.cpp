#include "include/w_mssq/w_mssq_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>

#include "w_mssq_plugin.h"

void WMssqPluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  w_mssq::WMssqPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
