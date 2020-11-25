import 'dart:async';

import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/material.dart';
import 'package:water_waste_management/Firebase/services.dart';
// import 'package:water_waste_management/Firebase/services.dart';
import 'package:water_waste_management/model/data_model.dart';

class MainBody extends StatefulWidget {

  @override
  _MainBody createState() => new _MainBody();
}



class _MainBody extends State<MainBody>{
  final ref = FirebaseDatabase.instance.reference().child('flow meter');
  
  SnapshotData items = new SnapshotData("0", "0", "0", "0");

  //StreamSubscription<Event> _onAddeds;
  StreamSubscription<Event> _onChangeds;
  void _onChangeddes( Event event){
    setState(() {
      if(event.snapshot.key == "flow rate"){
      items.flowrate = event.snapshot.value;
    }else if(event.snapshot.key == "discharge value"){
      items.dischargevalue = event.snapshot.value;
    }else if(event.snapshot.key == "distance"){
      items.distance = event.snapshot.value;
  
    }
    });
    
  
}

  @override
  void initState(){
    //_onAddeds = ref.onChildAdded.listen(_onAdded);
    
    _onChangeds = ref.onChildChanged.listen(_onChangeddes);

    super.initState();
  }

  @override
  void dispose(){
    //_onAddeds.cancel();
    _onChangeds.cancel();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
         backgroundColor: Colors.green[50],
        appBar: AppBar(
          backgroundColor: Colors.lightBlue[600],
          title: Text(
            'SMART WATER USE',
            textAlign: TextAlign.center,
            style: TextStyle(fontWeight: FontWeight.bold),
          ),
        ),
          body: SafeArea(
          child: Column(
              mainAxisAlignment: MainAxisAlignment.spaceEvenly,
              children: <Widget>[
                Center(
                  child: Container(
                    height: 80.0,
                    width: 400.0,
                    color: Colors.lightBlueAccent[400],
                    child: Text(
                      'Water Discharged: ${items.dischargevalue == null?"0":items.dischargevalue} L',
                      textAlign: TextAlign.center,
                      style: TextStyle(fontSize: 26.0),
                    ),
                  ),
                ),
                Center(
                  child: Container(
                    height: 80.0,
                    width: 400.0,
                    color: Colors.lightBlueAccent[400],
                    child: Text('Length of Empty Tank : ${items.distance == null?"0":items.distance} cm ',
                        textAlign: TextAlign.center,
                        style: TextStyle(fontSize: 26.0)),
                  ),
                ),
                Center(
                  child: Container(
                    height: 80.0,
                    width: 400.0,
                    color: Colors.lightBlueAccent[400],
                    child: Text('Flowrate : ${items.flowrate == null?"0":items.flowrate} LPM',
                        textAlign: TextAlign.center,
                        style: TextStyle(fontSize: 26.0)),
                  ),
                ),
              ]),
        )
    );
  }
}


/*class Body extends StatelessWidget {
  final SnapshotData items;
 

  const Body({Key key, this.snapshotData}) : super(key: key); 
  @override
  Widget build(BuildContext context) {
    print(snapshotData);
    return SafeArea(
          child: Column(
              mainAxisAlignment: MainAxisAlignment.spaceEvenly,
              children: <Widget>[
                Center(
                  child: Container(
                    height: 80.0,
                    width: 400.0,
                    color: Colors.lightBlueAccent[400],
                    child: Text(
                      'Water Discharged: ${items.dischargevalue} L',
                      textAlign: TextAlign.center,
                      style: TextStyle(fontSize: 26.0),
                    ),
                  ),
                ),
                Center(
                  child: Container(
                    height: 80.0,
                    width: 400.0,
                    color: Colors.lightBlueAccent[400],
                    child: Text('Distance : ${snapshotData.distance} cm ',
                        textAlign: TextAlign.center,
                        style: TextStyle(fontSize: 26.0)),
                  ),
                ),
                Center(
                  child: Container(
                    height: 80.0,
                    width: 400.0,
                    color: Colors.lightBlueAccent[400],
                    child: Text('Flowrate : ${snapshotData.flowrate} LPM',
                        textAlign: TextAlign.center,
                        style: TextStyle(fontSize: 26.0)),
                  ),
                ),
              ]),
        );
  }

}*/
  
