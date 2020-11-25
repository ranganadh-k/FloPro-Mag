
#include <FirebaseArduino.h>
#include <ArduinoJson.h>

#include<SoftwareSerial.h>

#include <EEPROM.h>

//YF-DN50
//NodeMCU 1.0 (ESP-12E)
//Last modified on 26-01-2019 by Dr.Rambabu

//---oled
// SDA - D2red
// SCL - D1siver
// VCC - 3V3yellow
// GND - GNDbue

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>



#define OLED_RESET D5
const int trigPin = D7;  //D4
const int echoPin = D8;  //D3
double duration;
double distance;
/* Object named display, of the class Adafruit_SSD1306 */
Adafruit_SSD1306 display(OLED_RESET);


uint8_t s = 0, m = 0, h = 0;

//wifi------------------
#include <ESP8266WiFi.h>

#include <WiFiClient.h> 
#include <ESP8266HTTPClient.h>

//-----------------
#define FIREBASE_HOST "water-use.firebaseio.com" 
#define FIREBASE_AUTH "dteuweJoFOCI7hsWtMpxjdV0aL8NRZkYnf63VGeU"

const char* ssid = "AJAY";
const char* password = "12345678";

int status = WL_IDLE_STATUS;

// Initialize the client library
WiFiClient client;

//byte statusLed    = 13;

byte sensorInterrupt = D5;  
byte sensorPin       = D5;

float calibrationFactor = 0.20; //0.45 for 40mm dia. and 0.2 for 50mm dia.

volatile long pulseCount=0;

float flowRate;
float flowLitres;
float totalLitres;
float H=100;

//double EEPROM_readDouble(int);

float prevTotalLitres=0;

unsigned long oldTime;

int counter=0;
int eepromcounter=0;
void setup()
{

  Serial.begin(115200);
  EEPROM.begin(512);  //Initialize EEPROM
 
 Serial.println("Attempting to connect to WPA network...");
  Serial.print("SSID: ");
  Serial.println(ssid);

  status = WiFi.begin(ssid, password);

  Serial.println(status);

  
  while (WiFi.status() != WL_CONNECTED  ) {
    
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  
 if (WiFi.status() == WL_CONNECTED)
 {
    Serial.println("Connected to WIFI");
 }
  Serial.println("");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  

 // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
   
  pulseCount   = 0;
  flowRate     = 0.0;
  flowLitres   = 0;

  totalLitres = EEPROM_readDouble(0);

  oldTime      = 0;
 
  pinMode(sensorPin, INPUT);
//  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);

  Serial.println(totalLitres);
  

 //Initialize OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); /* Initialize display with address 0x3C */
  display.clearDisplay();  /* Clear display */
  display.setTextSize(1);  /* Select font size of text. Increases with size of argument. */
  display.setTextColor(WHITE);  /* Color of text*/
  updateWatch();
  
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input  
}

//Start Wifi and Send Data to Server
//void SendDataToServer(String data)
void loop()
{
  
  String myString;
  String string1;
  String string;
  if (flowRate>0)
  {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(100);  
  }
  else
  {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);                       // wait for a second
  }
  
  if((millis() - oldTime) > 1000)    // Only process counters once per second
  {
    detachInterrupt(sensorInterrupt);
    
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    oldTime = millis();
    flowLitres = (flowRate / 60);
    totalLitres += flowLitres;     
    unsigned int frac;   
    Serial.print("Flow rate: ");
    Serial.print(int(flowRate));  // Print the integer part of the variable
    Serial.print(".");             // Print the decimal point
    frac = (flowRate - int(flowRate)) * 10;
    Serial.print(frac, DEC) ;      // Print the fractional part of the variable
    Serial.print("L/Min");
    Serial.print("  Current Liquid Flowing: ");             // Output separator
    Serial.print(flowLitres);
    Serial.print("L/Sec");
    Serial.print("  Output Liquid Quantity: ");             // Output separator
    Serial.print(totalLitres);
    Serial.println("L");

///////////EEPROM Write////////
      EEPROM_writeDouble(0,totalLitres); 
///////////////////////////////
   
    pulseCount = 0;
  }
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
//    /us....................................................................................................................................................
 // Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

// Calculating the distance
distance= duration/60;
// Prints the distance on the Serial Monitor
Serial.print("distance");
float h=(H-distance)*(100/H);
Serial.println(distance);
/////////////////////////////////////////////////////////////////////////////
    String data3="distance " +String(distance);
    String data1= "Flow Rate: " + String(flowRate) + " LPM";
    String data2= "Quantity: " + String(totalLitres) + " Litres";
   Serial.println(data1);
    Serial.println(data2);

    oledDrawString(data1,data2,data3);
    //updateWatch();/* Every second increment clock and display */
//firebase
myString=String(flowLitres);
Firebase.setString("flow meter/flow rate",myString);
string1=String(totalLitres);
Firebase.setString("flow meter/discharge value",string1);
string=String(distance);
Firebase.setString("flow meter/distance",string);


  //Added for eeprom***************************************************************************************************************************************
  

  // write appropriate byte of the EEPROM.
  // these values will remain there when the board is
  // turned off.
  
  
  //Write string to eeprom
   
   // if(eepromcounter==(60*5)){
     // eepromcounter=0;
   // /EEPROM_writeDouble(0,totalLitres ); //Write one by one with starting address of 0x0F
    //Serial.println("EEPROM>>>>>>");
    //Serial.print(EEPROM_readDouble(0));
   // }
   delay(1000) ;
  EEPROM.commit();

    
}

void  pulseCounter()
{
  pulseCount++;
}


void oledDrawString(String data1, String data2,String data3) {
  display.clearDisplay();

  display.setTextSize(1.5);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  display.println(data1);
  display.setCursor(0, 12);     // Start at top-left corner
  display.println(data2);
  display.setCursor(0,24);
  display.println(data3);
  
  display.display();
  delay(2000);
}

void updateWatch() {
  s = s + 1;
  if (s == 60) {
    s = 0;
    m = m + 1;
  }
  if (m == 60) {
    m = 0;
    h = h + 1;
  }
  if (h == 13)
    h = 1;

  writeOLED();
}

void writeOLED(){
  char time[30];
  display.clearDisplay();  /* Clear display */
  sprintf(time, "%02d:%02d:%02d", h, m, s);
  drawStr(50, 10, "Timer");
  drawStr(40, 30, time);
  drawStr(20, 50, "ElectronicWings");
  display.display();
}

void drawStr(uint8_t x, uint8_t y, char* str){
  display.setCursor(x, y);  /* Set x,y coordinates */
  display.println(str);
}

void EEPROM_writeDouble(int ee, double value)
{
   byte* p = (byte*)(void*)&value;
   for (int i = 0; i < sizeof(value); i++)
       EEPROM.write(ee++, *p++);
}

double EEPROM_readDouble(int ee)
{
   double value = 0.0;
   byte* p = (byte*)(void*)&value;
   for (int i = 0; i < sizeof(value); i++)
       *p++ = EEPROM.read(ee++);
   return value;
}
