import 'package:flutter/cupertino.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:w_mssq/w_mssq.dart';
import 'package:w_mssq/w_mssq_platform_interface.dart';
import 'package:w_mssq/w_mssq_method_channel.dart';

void main() {
  test('getPlatformVersion', () async {
    WidgetsFlutterBinding.ensureInitialized();
    WMssq wMssqPlugin = WMssq();
    WMssqPlatform fakePlatform = MethodChannelWMssq();
    WMssqPlatform.instance = fakePlatform;

    // expect(await wMssqPlugin.getPlatformVersion(), '42');
  });
}
