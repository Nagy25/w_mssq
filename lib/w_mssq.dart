import 'dart:async';

import 'package:flutter/services.dart';
import 'package:w_mssq/helpers/enums.dart';

import 'w_mssq_platform_interface.dart';

mixin WMssq {

  static Future<ConnectionResult> sqlConnect(List<dynamic> list) async {

    BackgroundIsolateBinaryMessenger.ensureInitialized(list[1] as RootIsolateToken);
    final result =
        await WMssqPlatform.instance.sqlConnect(serverName: list[0].toString());

    return ConnectionResult.valueOf(result);

  }

  static Future<void> close() async {
    return WMssqPlatform.instance.closeConnection();
  }

  static Future<List> execute(List<dynamic> l) async {

    BackgroundIsolateBinaryMessenger.ensureInitialized(l[1] as RootIsolateToken);
    final data = await WMssqPlatform.instance.execute(query: l[0].toString());
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
