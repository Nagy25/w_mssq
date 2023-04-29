import 'package:plugin_platform_interface/plugin_platform_interface.dart';

import 'w_mssq_method_channel.dart';

abstract class WMssqPlatform extends PlatformInterface {
  /// Constructs a WMssqPlatform.
  WMssqPlatform() : super(token: _token);

  static final Object _token = Object();

  static WMssqPlatform _instance = MethodChannelWMssq();

  /// The default instance of [WMssqPlatform] to use.
  ///
  /// Defaults to [MethodChannelWMssq].
  static WMssqPlatform get instance => _instance;

  /// Platform-specific implementations should set this with their own
  /// platform-specific class that extends [WMssqPlatform] when
  /// they register themselves.
  static set instance(WMssqPlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = instance;
  }

  Future<String> sqlConnect({required String serverName});

  Future<void> closeConnection();

  Future<dynamic> execute();
}
