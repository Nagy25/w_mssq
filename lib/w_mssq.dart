import 'w_mssq_platform_interface.dart';

class WMssq {
  Future<String?> sqlConnect() {
    return WMssqPlatform.instance.sqlConnect();
  }
}
