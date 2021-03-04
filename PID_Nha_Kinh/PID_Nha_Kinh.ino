/*
Code viet cho Esp8266 NodeMCU V3
https://bitly.com.vn/b6vwo7
GPIO 4, 5, 12, 13, 14, 16 có thể sử dụng bình thường.



Document: https://docs.blynk.cc/#blynk-firmware-virtual-pins-control
*/

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>    
#include <SimpleTimer.h>
 
SimpleTimer timer;
//char auth[] = "C39HJavn-IoVsrHbYA7ea3IqlKhGauFL";
char auth[] = "8TytNxie25BVKYolzRm_yLjF1vAGbrCA";     // Cai dat mang
char ssid[] = "Wifi Cua Tuan";                        // Cai dat mang
char pass[] = "";                                     // Cai dat mang

float h=0;      // V4 - Do am khong khi
float t=0;      // V5 - Nhiet do khong khi
float hum=0;    // V6 - Do am Dat 
int flag=0;

/*
 * V1 V2 : Chọn chế độ
 * V4 Do am khong khi
 * V5 Nhiet do khong khi
 * V6 Do am dat
 * v7 Kp
 * V8 Ki
 * V9 Kd
 */
int ledState = LOW;
unsigned long Ago = 0;
unsigned long Now = 0;
const long Set_time = 300;

void setup()
{
    Serial.begin(115200);
    Blynk.begin(auth, ssid, pass,"iot.htpro.vn", 8080);
    pinMode(LED_BUILTIN, OUTPUT);
    timer.setInterval(500, sendUptime);
}

void sendUptime()
{
  h = h+0.845 ;             // Đọc giá tri nhiệt độ, độ ẩm không khí
  t = t+1.23 ;              // Đọc giá tri nhiệt độ, độ ẩm không khí
  if (h>=100){              // Đọc giá tri nhiệt độ, độ ẩm không khí
    h=0;                    // Đọc giá tri nhiệt độ, độ ẩm không khí
  }                         // Đọc giá tri nhiệt độ, độ ẩm không khí
  if(t>=100){               // Đọc giá tri nhiệt độ, độ ẩm không khí
    t=1;                    // Đọc giá tri nhiệt độ, độ ẩm không khí
  }
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
  Serial.println("\nKet qua do duoc:");       // Hiển thị trên monitor
  Serial.print("Do Am = ");                     // Hiển thị trên monitor
  Serial.print(h);                              // Hiển thị trên monitor
  Serial.print("%  ");                          // Hiển thị trên monitor
  Serial.print("Nhiet do = ");                  // Hiển thị trên monitor
  Serial.print(t);                              // Hiển thị trên monitor
  Serial.print("°C \n");                        // Hiển thị trên monitor
  Blynk.virtualWrite(V6, t);      // Gửi dữ liệu lên sever - lên Điện thoại.
  Blynk.virtualWrite(V5, h);      // Gửi dữ liệu lên sever - lên Điện thoại.
  
  if(t>=70 && flag==0){
    Blynk.notify("Nhiệt độ cao");
    flag=1;
  }else if(t<70){
      flag=0;
    }  
}

void loop()
{
  Blynk.run();
  timer.run();
}
