/*
  Analog Input

  Demonstrates analog input by reading an analog sensor on analog pin 0 and
  turning on and off a light emitting diode(LED) connected to digital pin 13.
  The amount of time the LED will be on and off depends on the value obtained
  by analogRead().

  The circuit:
  - potentiometer
    center pin of the potentiometer to the analog input 0
    one side pin (either one) to ground
    the other side pin to +5V
  - LED
    anode (long leg) attached to digital output 13
    cathode (short leg) attached to ground

  - Note: because most Arduinos have a built-in LED attached to pin 13 on the
    board, the LED is optional.

  created by David Cuartielles
  modified 30 Aug 2011
  By Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/AnalogInput
*/

int sensorPin = A0;    // select the input pin for the potentiometer
int ledPin = 12;      // select the pin for the LED
int led = 2; 
int sensorValue = 0;  // variable to store the value coming from the sensor

void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);
    pinMode(led, OUTPUT);
    Serial.begin(115200);
}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
  int percent = map(sensorValue, 1024, 0, 0, 100);

  // turn the ledPin on
  digitalWrite(ledPin, HIGH);
    digitalWrite(led, HIGH);
  // stop the program for <sensorValue> milliseconds:
  delay(sensorValue);
  delay(500);
  // turn the ledPin off:
  digitalWrite(ledPin, LOW);
    digitalWrite(led, LOW);
  // stop the program for for <sensorValue> milliseconds:
  delay(sensorValue);

    // print the results to the Serial Monitor:
  Serial.print("sensor = ");
  Serial.print(sensorValue);
  Serial.print("\t  ");
    Serial.print(percent);
  Serial.println('%');
    Serial.print(" \n  ");

  // wait 2 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  delay(2);
}
