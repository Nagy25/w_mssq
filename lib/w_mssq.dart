import 'package:w_mssq/helpers/enums.dart';

import 'w_mssq_platform_interface.dart';

mixin WMssq {
  static Future<ConnectionResult> sqlConnect(
      {required String serverName}) async {
    final result =
        await WMssqPlatform.instance.sqlConnect(serverName: serverName);
    return ConnectionResult.valueOf(result);
  }

  static Future<void> close() async {
    return WMssqPlatform.instance.closeConnection();
  }

  static Future<List> execute({required String query}) async {
    final data = await WMssqPlatform.instance.execute(query: query);
    final list = [];
    for (final element in data) {
      Map cleanedElement = {};
      if (element is Map) {
        element.forEach((key, value) {
          cleanedElement[key] = value.toString().replaceAll('\u0000', '');
        });
        list.add(cleanedElement);
      } else {
        list.add(element.toString().replaceAll('\u0000', ''));
      }
    }

    return list;
  }
}
