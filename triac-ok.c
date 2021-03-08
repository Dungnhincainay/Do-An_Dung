
// https://circuitdigest.com/microcontroller-projects/arduino-ac-light-dimmer-using-triac



int LAMP = 4;

int dim_val=0;

void setup()

{

  pinMode(LAMP, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(2), zero_cross, CHANGE);

}

void zero_cross()

{

  int dimming_time = (200*dim_val);

  delayMicroseconds(dimming_time);  

  digitalWrite(LAMP, HIGH);

  delayMicroseconds(10);

  digitalWrite(LAMP, LOW);

}

void loop() 

{

    int data=analogRead(A0);

    int data1 = map(data, 0, 1023,10,49); 

    dim_val=data1;

}