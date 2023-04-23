import 'package:flutter_test/flutter_test.dart';
import 'package:w_mssq/w_mssq.dart';
import 'package:w_mssq/w_mssq_platform_interface.dart';
import 'package:w_mssq/w_mssq_method_channel.dart';
import 'package:plugin_platform_interface/plugin_platform_interface.dart';

class MockWMssqPlatform
    with MockPlatformInterfaceMixin
    implements WMssqPlatform {

  @override
  Future<String?> getPlatformVersion() => Future.value('42');
}

void main() {
  final WMssqPlatform initialPlatform = WMssqPlatform.instance;

  test('$MethodChannelWMssq is the default instance', () {
    expect(initialPlatform, isInstanceOf<MethodChannelWMssq>());
  });

  test('getPlatformVersion', () async {
    WMssq wMssqPlugin = WMssq();
    MockWMssqPlatform fakePlatform = MockWMssqPlatform();
    WMssqPlatform.instance = fakePlatform;

    expect(await wMssqPlugin.getPlatformVersion(), '42');
  });
}
