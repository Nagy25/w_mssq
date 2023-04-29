import 'package:flutter/services.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:w_mssq/w_mssq_method_channel.dart';

void main() {
  MethodChannelWMssq platform = MethodChannelWMssq();
  const MethodChannel channel = MethodChannel('w_mssq');

  TestWidgetsFlutterBinding.ensureInitialized();

  setUp(() {
    channel.setMockMethodCallHandler((MethodCall methodCall) async {
      return '42';
    });
  });

  tearDown(() {
    channel.setMockMethodCallHandler(null);
  });
}
