
#include <TimerOne.h>
#define Sensor A0
#define Fan 6
#define Lamp 5
float celsius, celsius_set =0, last_celsius;
float err, err_old, PW1, PW2;
volatile float DUTY;
//float Kp = 2, Ki = 0.065, Kd = 3.5, P, D, I;
float kp = 9; Ki = 0.005, Kd = 6.3, P, D, I;
int mode = -1;
float reading;
float voltage;
void setup() 
{
  Serial.begin(9600);
  pinMode(Fan,OUTPUT);
  Timer1.initialize(100000);
  Timer1.attachInterrupt(GetTemp);
  Serial.setTimeout(5000);
  PW1 = 0;
  PW2 = 0;
  reading = 0;
  Serial.println('a');
  char a = 'b';
  while (a !='a')
  { a = Serial.read();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
if (Serial.available()>0)
{
  mode = Serial.read();
  switch(mode)
  {
    case 'T':  //GetTemp();
               Serial.println(celsius);
               break;
    case 'R':  celsius_set = Serial.parseInt();
               break;
  }
}
}
void GetTemp(void)    //hieu chinh thong so pid
{
  for (int i = 0; i<100; i++)
  {
    reading = reading + analogRead(sensor);
  }
  reading = reading/100;
  voltage = reading * 5.0 / 1024.0;
  celsius = voltage *100.0;
  err = celsius_set - cesius;
  //P
  P = Kp*err;
  //I
  I = Ki*err*100/1000;
  //D
  D = Kd*(err - err_old)*10;
  err_old = err;
  DUTY = P + I + D;
  delay(5);
  PW1 = PW1 + DUTY;
  PW2 = PW2 + DUTY;
  PW1 = constrain(PW1,0,255);
  PW2 = constrain(PW2,0,255);
  analogWrite(Fan,PW2);
  analogWrite(Lamp,PW1);
}
  
  }
}
}
