import 'w_mssq_platform_interface.dart';

class WMssq {
  Future<String?> sqlConnect({required String serverName}) {
    return WMssqPlatform.instance.sqlConnect(serverName: serverName);
  }

  Future<void> close() async {
    return WMssqPlatform.instance.closeConnection();
  }
}
