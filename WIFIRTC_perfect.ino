
/*
  MKR1000 - MKR WiFi 1010 - MKR VIDOR 4000 WiFi RTC

  This sketch asks NTP for the Linux epoch and sets the internal Arduino MKR1000's RTC accordingly.

  updated/modified by Ranganadh K
  <ranganadh.kollapureddi@gmail.com>

  3rd March, 2020

  http://arduino.cc/en/Tutorial/WiFiRTC
  This code is in the public domain.
*/

#include <SPI.h>
#include <WiFi101.h>
//#include <WiFiNINA.h> //Include this instead of WiFi101.h as needed
#include <WiFiUdp.h>
#include <RTCZero.h>

RTCZero rtc;

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = "JioFi_201F0E3";        // your network SSID (name)
char pass[] = "y0fjqi0479";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                           // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

 const int GMT = 5; 
//change this to adapt it to your time zone, however, as it doesn't take float values 
//for IST i.e. 5.30 cannot be added. for which has been supported further in the code.

void setup() {
  Serial.begin(115200);

  // check if the WiFi module works
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the status:
  printWiFiStatus();

  rtc.begin();

  unsigned long epoch;
  int numberOfTries = 0, maxTries = 6;
  do {
    epoch = WiFi.getTime();
    numberOfTries++;
  }
  while ((epoch == 0) || (numberOfTries < maxTries));  // can use && instead of || in case of errors

  if (numberOfTries > maxTries) {
    Serial.print("NTP unreachable!!");
    while (1);
  }
  else {
    Serial.print("Epoch received: ");
    Serial.println(epoch);
    
    rtc.setEpoch(epoch+ 88200);
    // +88200 corresponds to sum of 1800 (which adds 30mins to fit IST) and 
    // +86,400 (adds 1 day to sum Feb 29th Leap Year error).

    Serial.println();
  }
}

void loop() {
  printDate();
  printTime();
//  Serial.println( " Epoch=" + epoch);
  Serial.println();
  delay(1000);
}

void printTime()
{
  print2digits(rtc.getHours()+ GMT);
  Serial.print(":");
  print2digits(rtc.getMinutes());
  Serial.print(":");
  print2digits(rtc.getSeconds());
  Serial.println();
}

void printDate()
{
  Serial.print(rtc.getDay());
  Serial.print("/");
  Serial.print(rtc.getMonth());
  Serial.print("/");
  Serial.print(rtc.getYear());

  Serial.print(" ");
}


void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void print2digits(int number) {
  if (number < 10) {
    Serial.print("0");
  }
  Serial.print(number);
}
