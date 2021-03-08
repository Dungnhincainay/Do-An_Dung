
#include <ESP8266WiFi.h>
int LAMP[2] = {13,12};
int dimming[2] = {125,125};

void setup()
{
Serial.begin(115200);
pinMode(LAMP[0], OUTPUT);
pinMode(LAMP[1], OUTPUT);
attachInterrupt(14, zero_crosss, RISING);
pinMode(5, INPUT);
pinMode(4, INPUT);
}


void zero_crosss()
{

for(int i=0; i < 2; i++)
{
int dimtime = (65*dimming[i]);
delayMicroseconds(dimtime);
digitalWrite(LAMP[i], HIGH);
delayMicroseconds(8.33);
digitalWrite(LAMP[i], LOW);
}

}





void loop()
{
if(digitalRead(5)==0)
{
yield;
if(dimming[0] == 0)
{
dimming[0] = 125;
}
int Milliseconds_1;

Milliseconds_1 = millis()+100;
while (Milliseconds_1 > millis()) ;
dimming[1]= dimming[0] -5;
Serial.println(dimming[0]);
}



if(digitalRead(4)==0)
{
yield;
if(dimming[1] == 0)
{
dimming[1] = 125;
}
int Milliseconds_2;
Milliseconds_2 = millis()+100;
while (Milliseconds_2 > millis()) ;
dimming[1]= dimming[1] -5;
Serial.println(dimming[1]);
}
}