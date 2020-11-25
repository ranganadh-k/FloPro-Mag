const int analogInPin=A0;
float sensorValue=0;
float outputValue=0;
float flowvalue=0;
float prevflowvalue=0;
void setup()
{
  Serial.begin(9600);
  flowvalue=prevflowvalue;
}

void loop()
{
  int i=0;
  sensorValue=analogRead(analogInPin);
  outputValue=map1(sensorValue,269, 500, 0,11.5 );
  Serial.println("flow rate="+String(outputValue)+"m^3/hr");
  flowvalue=flowvalue+(outputValue/3600);
  Serial.println("total quantity="+String(flowvalue)+"m^3");
  
  
  delay(1000);
}
float map1(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
