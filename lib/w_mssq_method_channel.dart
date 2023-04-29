import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';

import 'w_mssq_platform_interface.dart';

/// An implementation of [WMssqPlatform] that uses method channels.
class MethodChannelWMssq extends WMssqPlatform {
  /// The method channel used to interact with the native platform.
  @visibleForTesting
  final methodChannel = const MethodChannel('w_mssq');

  @override
  Future<String> sqlConnect({required String serverName}) async {
    final data = await methodChannel
        .invokeMethod<String>('sqlConnection', {'serverName': serverName});
    return data.toString();
  }

  @override
  Future<void> closeConnection() async {
    await methodChannel.invokeMethod('closeConnection');
  }

  @override
  Future<List> execute({required String query}) async {
    return await methodChannel.invokeMethod('sqlQuery', {'query': query});
  }
}
