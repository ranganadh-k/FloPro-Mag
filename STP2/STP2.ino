//1000
#include <Firebase_Arduino_WiFi101.h>
#include <Firebase_Arduino_WiFi101_HTTPCLient.h>

#include <SPI.h>
#include <SD.h>
const int chipSelect = 4;

#include <WiFi101.h>
char const * SSID_NAME = "AJAY"; // Put here your SSID name
char const * SSID_PASS = "12345678"; // Put here your password
const int analogInPin=A0;
float sensorValue=0;
float outputValue=0;
float totalFlow=0;
float prevTotalFlow;

#define FIREBASE_HOST "stpiot.firebaseio.com"
#define FIREBASE_AUTH "kV3DiGbdP7OH9SsKyZvfXsDShmPZJqqb52MaJb6p"
int status = WL_IDLE_STATUS;
FirebaseData firebaseData;
WiFiSSLClient client;

 void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, SSID_NAME, SSID_PASS);
  Firebase.reconnectWiFi(true);
  totalFlow=prevTotalFlow;
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  ////////////////SD Card/////////////////////////
 /* while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card...");
   if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");*/
  ////////////////////////////////////////////////
   Serial.print("Connecting to Wi-Fi");
  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED)
  {
    status = WiFi.begin(SSID_NAME, SSID_PASS);
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
  Serial.println();
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 /*  // check for the WiFi module:
  if (status != WL_CONNECTED) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }
  
  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SSID_NAME);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(SSID_NAME, SSID_PASS);
    // wait 10 seconds for connection:
    delay(10000);
  } */
//////////////////////////////////////////////////////////////////////////////////////////////
  Serial.println("Connected to wifi");
  printWiFiStatus();
}
void loop(){
  ////sensor values
  sensorValue=analogRead(analogInPin);
 outputValue=mapfun(sensorValue,269, 500, 0,10.9 );
  Serial.println(outputValue);
 Serial.println(totalFlow);
 

 //check for internet connection
 //SD Card///////////////////////////////

 // File dataFile = SD.open("dataSTP.txt", FILE_WRITE);

  // if the file is available, write to it:
 // if (dataFile) {
   // dataFile.println(totalFlow);
    //dataFile.close();
    // print to the serial port too:
    //Serial.println(totalFlow);
 // }
  // if the file isn't open, pop up an error:
 // else {
  //  Serial.println("error opening dataSTP.txt");
  //}
 ///////////////////////////////////////
 
 
  // attempt to connect to WiFi network:
  if (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SSID_NAME);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(SSID_NAME, SSID_PASS);
    // wait 10 seconds for connection:
    //delay(10000);
  }
  
 else{
  Firebase.setString(firebaseData,"flow rate",String(outputValue));
  Firebase.setString(firebaseData,"discharge value",String(totalFlow));
 }
 
 totalFlow=totalFlow+(outputValue/3600);
  delay(1000);
  
    
}
float mapfun(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
