import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';

import 'w_mssq_platform_interface.dart';

/// An implementation of [WMssqPlatform] that uses method channels.
class MethodChannelWMssq extends WMssqPlatform {
  /// The method channel used to interact with the native platform.
  @visibleForTesting
  final methodChannel = const MethodChannel('w_mssq');

  @override
  Future<String?> getPlatformVersion() async {
    final version =
        await methodChannel.invokeMethod<String>('getPlatformVersion');
    return version;
  }

  @override
  Future<String> getString() async {
    print('begin');
    final data = await methodChannel
        .invokeMethod<String>('returnString', {'input': 'this is my data'});
    return data.toString();
  }
}
