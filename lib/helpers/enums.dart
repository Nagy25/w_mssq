import 'package:collection/collection.dart';

enum ConnectionResult {
  connected,
  environmentError,
  odbcError,
  connectionError,
  sqlConnectionError,
  undefinedError;

  const ConnectionResult();

  static ConnectionResult valueOf(String? name) =>
      ConnectionResult.values
          .firstWhereOrNull((element) => element.name == name) ??
      ConnectionResult.undefinedError;
}
