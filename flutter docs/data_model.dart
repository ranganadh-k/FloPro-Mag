

import 'package:firebase_database/firebase_database.dart';

class SnapshotData{
 String id;
 String flowrate;
 String dischargevalue;
 String distance;
  SnapshotData(this.id,this.flowrate, this.dischargevalue, this.distance);

 SnapshotData.fromSnapshot(DataSnapshot snapshot){
      id = snapshot.key;
      flowrate =  snapshot.value['flow rate'].toString();
      distance =  snapshot.value['distance'].toString();
      dischargevalue = snapshot.value['discharge value'].toString();
  }

}

