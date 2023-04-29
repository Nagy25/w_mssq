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
  String _data = '';
  final TextEditingController _controller = TextEditingController();

  @override
  void initState() {
    super.initState();
  }

  Future<void> _startConnect() async {
    final data =
        await WMssq.sqlConnect(serverName: 'AHMED-NAGY\\MSSQLSERVER01') ??
            'Unknown platform version';
    _result = data;
    setState(() {});
  }

  void _close() {
    WMssq.close();
  }

  void _execute(String query) async {
    final l = await WMssq.execute(query: query);
    print(l.length);
    l.forEach((element) {
      print(element);
    });
    _data = l[0]["ID"] + " - " + l[0]['MachineName'];
    setState(() {});
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
                  onPressed: _startConnect, child: const Text('connect')),
              const SizedBox(
                height: 20,
              ),
              ElevatedButton(
                  onPressed: _close, child: const Text('disconnect')),
              const SizedBox(
                height: 100,
              ),
              Text('Running on: $_result\n'),
              TextField(
                controller: _controller,
              ),
              const SizedBox(
                height: 100,
              ),
              ElevatedButton(
                  onPressed: () => _execute(_controller.text),
                  child: const Text('getData')),
              Text('data: $_data'),
            ],
          ),
        ),
      ),
    );
  }
}
