import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'dart:async';
import 'package:w_mssq/w_mssq.dart';

void main() {
  WidgetsFlutterBinding.ensureInitialized();
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({super.key});

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  String _result = '';
  List<dynamic> _data = [];
  final TextEditingController _controller = TextEditingController();

  @override
  void initState() {
    super.initState();
  }

  RootIsolateToken rootToken = RootIsolateToken.instance!;
  Future<void> _startConnect() async {

     final s = await compute( WMssq.sqlConnect, ["AHMED-NAGY\\MSSQLSERVER01",rootToken]);
     _result = s.name;
    setState(() {});
  }

  void _close() {
    WMssq.close();
  }



  void _execute(String query) async {

     final d = await  compute( WMssq.execute, [query,rootToken]);

    _data = d;
    setState(() {});

  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Plugin example app'),
        ),
        body: SingleChildScrollView(
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
                  onPressed: () async{
                    _execute("select TOP 200 * from pfx.dbo.Items");
                  },
                  child: const Text('getData from compute')),
             ... _data.map((e) => Column(
               children: [
                 Text(e.toString()),
                 const Divider(),
               ],
             )).toList()
            ],
          ),
        ),
      ),
    );
  }
}
