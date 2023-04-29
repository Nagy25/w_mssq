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
  }

  Future<void> startConnect() async {
    final data = await _wMssqPlugin.sqlConnect(
            serverName: 'AHMED-NAGY\\MSSQLSERVER01') ??
        'Unknown platform version';
    _result = data;
    setState(() {});
  }

  void close() {
    _wMssqPlugin.close();
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Plugin example app'),
        ),
        body: SizedBox(
          width: 400,
          child: Column(
            children: [
              ElevatedButton(
                  onPressed: startConnect, child: const Text('connect')),
              const SizedBox(
                height: 20,
              ),
              ElevatedButton(onPressed: close, child: const Text('disconnect')),
              const SizedBox(
                height: 100,
              ),
              Text('Running on: $_result\n'),
            ],
          ),
        ),
      ),
    );
  }
}
