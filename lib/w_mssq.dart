
import 'w_mssq_platform_interface.dart';

class WMssq {
  Future<String?> getPlatformVersion() {
    return WMssqPlatform.instance.getPlatformVersion();
  }
}
