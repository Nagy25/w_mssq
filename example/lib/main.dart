import 'package:flutter/material.dart';
import 'dart:async';
import 'package:w_mssq/w_mssq.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({super.key});

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  String _result = '';
  final _wMssqPlugin = WMssq();

  @override
  void initState() {
    super.initState();
    initSqlConnect();
  }

  Future<void> initSqlConnect() async {
    final data = await _wMssqPlugin.sqlConnect() ?? 'Unknown platform version';
    _result = data;
    setState(() {});
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Plugin example app'),
        ),
        body: Center(
          child: Text('Running on: $_result\n'),
        ),
      ),
    );
  }
}
