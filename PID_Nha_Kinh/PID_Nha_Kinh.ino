
/* Code viet cho Esp8266 NodeMCU V3
https://bitly.com.vn/b6vwo7
GPIO 4, 5, 12, 13, 14, 16 có thể sử dụng bình thường.
    D2 D1  D6  D7 D5  D0

Document: https://docs.blynk.cc/#blynk-firmware-virtual-pins-control
http://rancilio-pid.de/5-der-erste-test/

Máy chủ của bạn là: iot.htpro.vn , blynk.remoteapp.de hoặc blynk.quynhanmobile.com
Cổng: 9443 trên app và 8080 tren app Blynk*/

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>    
#include <DHT.h>              // https://github.com/adafruit/DHT-sensor-library
SimpleTimer timer;

char auth[] = "C39HJavn-IoVsrHbYA7ea3IqlKhGauFL";     // Cai dat mang
//char auth[] = "8TytNxie25BVKYolzRm_yLjF1vAGbrCA";   // Cai dat mang
char ssid[] = "Wifi Cua Tuan";                        // Cai dat mang
char pass[] = "";                                     // Cai dat mang

// Khai báo chân kết nối
#define QUAT 5                      //GPIO5 - D1
#define Thanh_Nhiet 4               //GPIO4 - D2
#define BOM 12                      // D6
#define DHTPIN 13                   // D7              
#define INTERRUPT_PIN 14            // D5_ Sử dụng GPIO14 để ngat

#define DHTTYPE DHT21 // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);
float temp_sensor,humidity_sensor,hum_ADC,ADC_VALUE;  //  V5 - Do am khong khi, V6 - Nhiet do khong khi, V4 - Do am Dat 
float h,t,Nhietdodat,Do_am_dat,Sai_so;  

float A,A1,A2,Alpha,Beta,Gama,pid_Output,pid_LastOutput,Timer_1;
float led_T=0.05; // Lưu ý T- Thời gian trích mẫu

 int Che_do;
 float Kp =1;      //Kp
 float Ki =1;      //Ki
 float Kd =1;      //Kd


int FLAG_MODE,FLAG_PID_QUAT,FLAG_PID_NHIET_DO,FLAG_MANUAL;
int Toc_do_quat,value,Bat_bom,Bat_Thanh_Nhiet;

int flag=0;
int ledState = LOW;
unsigned long Ago = 0;
unsigned long Now = 0;
const long Set_time = 300;

WidgetLED led_Bom(V22);
WidgetLED led_Thanh_Nhiet(V21);
BLYNK_CONNECTED() {
    Blynk.syncAll();
}
 BLYNK_WRITE(V1){             // Chọn chế độ 1- PID, 2- Manual
   Che_do = param.asFloat(); // Nhận giá trị từ điện thoại
   if(Che_do==2){           // Tự động vào chế độ PID
     FLAG_MANUAL=1;
     FLAG_PID_NHIET_DO=0;
     FLAG_PID_QUAT=0;
     FLAG_MODE=0;
   }else{                   // Tự động vào chế độ PID
     FLAG_MANUAL=0;
     FLAG_PID_QUAT=1;
     FLAG_PID_NHIET_DO=1;
     FLAG_MODE=1;
   }
 }
 BLYNK_WRITE(V7){   
   Kp = param.asFloat(); // Nhận giá trị KP
 }
 BLYNK_WRITE(V8){   
   Ki = param.asFloat(); // Nhận giá trị KI
 }
 BLYNK_WRITE(V9){   
   Kd = param.asFloat(); // Nhận giá trị KD
 }
 BLYNK_WRITE(V10){   
   Toc_do_quat = param.asInt(); // Get value as integer
   value= map(Toc_do_quat,0,100,0,1023);
 }
 BLYNK_WRITE(V12){   
   Bat_bom = param.asInt();     // Get value as integer
 }
 BLYNK_WRITE(V11){   
   Bat_Thanh_Nhiet = param.asInt(); // Get value as integer
 }
 BLYNK_WRITE(V15){   
   Do_am_dat = param.asInt();     // Get value as integer
 }
 BLYNK_WRITE(V16){   
   Nhietdodat = param.asInt();     // Get value as integer
 }


void sendUptime()
{
  h = h+0.845 ;             // Test giá trị nhiệt độ, độ ẩm
  t = t+1.23 ;              // Test giá trị nhiệt độ, độ ẩm
  if (h>=100){              // Test giá trị nhiệt độ, độ ẩm
    h=0;                    // Test giá trị nhiệt độ, độ ẩm
  }                         // Test giá trị nhiệt độ, độ ẩm
  if(t>=100){t=1;}

  humidity_sensor = dht.readHumidity();         //doc gia tri do am tu  DHT21
  temp_sensor = dht.readTemperature();          // Doc gia tri nhiet do tu DHT21
  if (isnan(humidity_sensor) || isnan(temp_sensor)) {
      Serial.println("Loi khong co ket noi den DHT21!");
//      Blynk.notify("Lỗi Cảm biến nhiệt độ"); // Tạo cảnh báo
      humidity_sensor = 0;       
      temp_sensor = 0;          
    }

  Serial.println("\nKet qua do duoc:");         // Hiển thị trên monitor
  Serial.print("Do Am = ");                     // Hiển thị trên monitor
  Serial.print(humidity_sensor);                // Hiển thị trên monitor
  Serial.print("%  ");                          // Hiển thị trên monitor
  Serial.print("Nhiet do = ");                  // Hiển thị trên monitor
  Serial.print(temp_sensor);                    // Hiển thị trên monitor
  Serial.print("°C \n");                        // Hiển thị trên monitor
  Blynk.virtualWrite(V6, t);      // Gửi dữ liệu lên sever - lên Điện thoại.
  Blynk.virtualWrite(V5, h);      // Gửi dữ liệu lên sever - lên Điện thoại.
  
  if(t>=70 && flag==0){
//    Blynk.notify("Nhiệt độ cao"); // Tạo cảnh báo
    flag=1;
  }else if(t<70){
      flag=0;
    }  

  ADC_VALUE = analogRead(A0);                 // Doc gia tri do am dat
  hum_ADC =((ADC_VALUE/1024)/0.767442)*100;   //map(value, fromLow, fromHigh, toLow, toHigh)
  Blynk.virtualWrite(V4, hum_ADC);            // Hien thi gia tri do am dat len Blynk

  // Mã nguồn xử lý chế độ Manual điều khiển thiết bị bằng tay
  if(FLAG_MANUAL==1){   //VAO CHE DO MANUAL
    analogWrite(QUAT,value);  // Đieu khien toc do quat
    
    Blynk.virtualWrite(V10, Toc_do_quat);

    if (Bat_bom == 1){
      digitalWrite(BOM,HIGH);       // bat bom
      Serial.println("BAT BOM");    // Bat Bom
      led_Bom.on();
    }else {
      digitalWrite(BOM,LOW);        // tat bom
      Serial.println("TAT BOM");    // tat bom
      led_Bom.off();
    }
  
    if (Bat_Thanh_Nhiet == 1){
      Serial.println("BAT THANH NHIET");    // Bat thanh nhiet
      digitalWrite(Thanh_Nhiet,HIGH);                // Bat thanh nhiet

      led_Thanh_Nhiet.on();
    }else {
      Serial.println("TAT THANH NHIET");    // Tat thanh nhiet
      digitalWrite(Thanh_Nhiet,LOW);                 // Tat thanh nhiet

      led_Thanh_Nhiet.off();
    }
//    FLAG_MANUAL=0;  
  }
 
}

// Mã nguồn xử lý điều khiển Triac
void ICACHE_RAM_ATTR TriacControl(){ //them ICACHE_RAM_ATTR
  Serial.println("DA VAO CHUONG TRINH NGAT");
  if(FLAG_MODE==1){
    delayMicroseconds(Timer_1 * 10000);
    digitalWrite(Thanh_Nhiet, HIGH);
    delayMicroseconds(100);
    digitalWrite(Thanh_Nhiet, LOW);
   }
}

void setup()
{
    Serial.begin(115200);
    Blynk.begin(auth, ssid, pass,"iot.htpro.vn", 8080);
    pinMode(INTERRUPT_PIN, INPUT_PULLUP);
    attachInterrupt(INTERRUPT_PIN, TriacControl, FALLING);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(QUAT, OUTPUT);
    pinMode(BOM, OUTPUT);
    pinMode(Thanh_Nhiet, OUTPUT);
    timer.setInterval(1000, sendUptime);
}

void loop()
{
  Blynk.run();
  timer.run();


// Mã nguồn xử lý tính toán delay trước khi mở Triac
if(FLAG_PID_NHIET_DO==1){  ///VAO CHE DO PID NHIET DO
  A = Nhietdodat - temp_sensor;
  Serial.println("Day la che do PID, các thông số: ");
  Serial.println(Kp);
  Serial.println(Ki);
  Serial.println(Kd);
  if (A < 0) {
    Timer_1 = 0.9;
    }
    else{
      Alpha = 2*led_T*Kp + Ki*led_T*led_T + 2*Kd;
      Beta = led_T*led_T*Ki - 4*Kd - 2*led_T*Kp;
      Gama = 2*Kd;

      pid_Output = (Alpha*A + Beta*A1 + Gama*A2 + 2*led_T*pid_LastOutput) / (2*led_T);
      pid_LastOutput = pid_Output;
      A2=A1;
      A1=A;
  
      if (pid_Output>255){
        pid_Output = 255;
      }else{
          if (pid_Output<1){
              pid_Output = 1;
          }
        }   
      Timer_1 = 1/pid_Output;
      if (Timer_1<0){
        Timer_1 = 0;
      }
      else{
        if (Timer_1>0.9){
          Timer_1 = 0.9;
        }
      }
    }	  
	//FLAG_PID_NHIET_DO=0;  
}

  // Mã nguồn xử lý chế độ điều khiển cho Quạt
  if(FLAG_PID_QUAT==1){  ////VAO CHE DO TU DONG CHO QUAT
    Serial.println("Bat dau CHAY TU DONG CHO QUAT");
    Serial.println(Nhietdodat);
    Sai_so = temp_sensor - Nhietdodat;
    if (Sai_so < 0) {
       analogWrite(QUAT, 0);    // Tat quat
       Serial.println("analogWrite(QUAT, 0)");
    }
    else 
      if (Sai_so >= 5){
        analogWrite(QUAT, 1024);// Quat chay het cong suat
        Serial.println("analogWrite(QUAT, 1024)");
      }
      else 
        if (Sai_so >= 3 && Sai_so < 5){
          analogWrite(QUAT, 900);
          Serial.println("analogWrite(QUAT, 900)");
        }
        else 
          if (Sai_so >= 1 && Sai_so < 3){
          analogWrite(QUAT, 800);
          Serial.println("analogWrite(QUAT, 800)");
          }
          else {
            analogWrite(QUAT, 550);
            Serial.println("analogWrite(QUAT, 550)");
          }
  //  FLAG_PID_QUAT=0;
  }//Ket thuc ham chay tu dong cho quat

    Now = millis();                 // Nhấp nháy led
  if (Now - Ago >= Set_time) {    // Nhấp nháy led
    Ago = Now;                    // Nhấp nháy led
    if (ledState == LOW) {        // Nhấp nháy led
      ledState = HIGH;            // Nhấp nháy led
    } else {                      // Nhấp nháy led
      ledState = LOW;             // Nhấp nháy led
    }                             // Nhấp nháy led
    digitalWrite(LED_BUILTIN, ledState);       // Đảo trạng thái led
  }
 
} // ket thuc void loop
