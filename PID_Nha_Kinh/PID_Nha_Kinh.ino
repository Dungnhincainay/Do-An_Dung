
/* Code viet cho Esp8266 NodeMCU V3
    https://bitly.com.vn/b6vwo7
GPIO 4, 5, 12, 13, 14, 16 có thể sử dụng bình thường.
    D2 D1  D6  D7 D5  D0

Document: https://docs.blynk.cc/#blynk-firmware-virtual-pi1ns-control
          http://rancilio-pid.de/5-der-erste-test/

Máy chủ của bạn là: iot.htpro.vn , blynk.remoteapp.de hoặc blynk.quynhanmobile.com
Cổng: 9443 trên app Blynk và 8080 tren ESP */

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>    
#include "DHT.h"         // https://github.com/adafruit/DHT-sensor-library
SimpleTimer timer;

char auth[] = "C39HJavn-IoVsrHbYA7ea3IqlKhGauFL";   // Cai dat mang
char ssid[] = "P 402 2.4G";                        // Tên wifi (chính xac từng dấu cách, chữ hoa, thường)
char pass[] = "12345678";                          // Password (nếu k đặt pass thì bỏ chống)


  
// Khai báo chân kết nối
#define INTERRUPT_PIN 5             // D1_ Sử dụng GPIO14 để ngat
#define Thanh_Nhiet 4               //GPIO4 - D2
#define QUAT 14                     // D5
#define BOM 12                      // D6
#define DHTPIN 13                   // D7 - Kết nối chân DATA cảm biến nhiệt độ+ độ ẩm DHT11,DHT21             

#define DHTTYPE DHT11     // sử dụng cảm biến nào thì thay ở đây...DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);
float Temp_Sensor,Humidity_Sensor,Hum_ADC,ADC_VALUE;  //  V5 - Do am khong khi, V6 - Nhiet do khong khi, V4 - Do am Dat 
float h,t,Nhietdodat,Do_am_dat,Sai_so;                // Các biến xử lý giá trị cảm biến

float A,A1,A2,Alpha,Beta,Gama,pid_Output,pid_LastOutput,Timer_1;  //Biến tính toán PID (cần xem lại)

float Time_sample ;      // Lưu ý T- Thời gian trích mẫu.... Phải xem xét lại

float Kp =1;      //Kp
float Ki =0;      //Ki
float Kd =0;      //Kd

int Che_do,FLAG_MODE,FLAG_PID_QUAT,FLAG_PID_NHIET_DO,FLAG_MANUAL;
int Toc_do_quat,value,Bat_bom,Bat_Thanh_Nhiet,Do_Sang_Den,Timer_2;

int flag=0;
int ledState = LOW;
unsigned long Ago = 0;
unsigned long Now = 0;
const long Set_time = 300; // Thời gian đèn led nhấp nháy

WidgetLED led_Bom(V22);
WidgetLED led_Thanh_Nhiet(V21);

BLYNK_CONNECTED() {
  Blynk.syncAll();      // Đồng bộ trạng thái các nút ấn
 }
 BLYNK_WRITE(V1){             // Chọn chế độ 1- PID, 2- Manual
   Che_do = param.asInt(); // Nhận giá trị từ điện thoại
   }
BLYNK_WRITE(V3){   
   Time_sample = param.asFloat(); // Thoi gian trich mau
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
   value= map(Toc_do_quat,0,100,0,255);     // Quy đổi giá trị từ 0-100% thành 0-255
        //map(value, fromLow, fromHigh, toLow, toHigh);
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
 BLYNK_WRITE(V31){   
   Do_Sang_Den = param.asInt(); // Get value as integer
   Timer_2 = map(Do_Sang_Den,0,100,9000,0);
 }

void sendUptime()       //
{
  // h = h+0.845 ;             // Test giá trị nhiệt độ, độ ẩm, mua cảm biến về lắp thì xoá bỏ
  // t = t+1.23 ;              // Test giá trị nhiệt độ, độ ẩm, mua cảm biến về lắp thì xoá bỏ
  // if (h>=100){              // Test giá trị nhiệt độ, độ ẩm, mua cảm biến về lắp thì xoá bỏ
  //   h=0;                    // Test giá trị nhiệt độ, độ ẩm, mua cảm biến về lắp thì xoá bỏ
  // }                         // Test giá trị nhiệt độ, độ ẩm, mua cảm biến về lắp thì xoá bỏ
  // if(t>=100){t=1;}

  Humidity_Sensor = dht.readHumidity();         // Doc gia tri do am tu  DHT21
  Temp_Sensor =  dht.readTemperature();         // Doc gia tri nhiet do tu DHT21
   if (isnan(Humidity_Sensor) || isnan(Temp_Sensor)) {
      Serial.println("Loi khong co ket noi den DHT21!");
    //Blynk.notify("Lỗi Cảm biến nhiệt độ"); // Tạo cảnh báo       
  }

  // Serial.println("\nKet qua do duoc:");         // Hiển thị trên monitor ( Mở arduino IDE và ấn "Ctrl + Shift + M ") 
  // Serial.print("Do Am = ");                     // Hiển thị trên monitor ( Mở arduino IDE và ấn "Ctrl + Shift + M ")
  // Serial.print(Humidity_Sensor);                // Hiển thị trên monitor ( Mở arduino IDE và ấn "Ctrl + Shift + M ")
  // Serial.print("%  ");                          // Hiển thị trên monitor ( Mở arduino IDE và ấn "Ctrl + Shift + M ")
  // Serial.print("Nhiet do = ");                  // Hiển thị trên monitor ( Mở arduino IDE và ấn "Ctrl + Shift + M ")
  // Serial.print(Temp_Sensor);                    // Hiển thị trên monitor ( Mở arduino IDE và ấn "Ctrl + Shift + M ")
  // Serial.print("°C \n");                        // Hiển thị trên monitor ( Mở arduino IDE và ấn "Ctrl + Shift + M ")

  Blynk.virtualWrite(V6, Temp_Sensor);          // Gửi dữ liệu lên điện thoại
  Blynk.virtualWrite(V5, Humidity_Sensor);      // Gửi dữ liệu lên điện thoại
  
  if(Temp_Sensor>=70 && flag==0){
//    Blynk.notify("Nhiệt độ cao"); // Tạo cảnh báo
    flag=1;
  }else if(Temp_Sensor<70){
      flag=0;
    }  

  ADC_VALUE = analogRead(A0);                 // Doc gia tri do am dat
  Hum_ADC =((ADC_VALUE/1024)/0.767442)*100;   //map(value, fromLow, fromHigh, toLow, toHigh)
  Blynk.virtualWrite(V4, Hum_ADC);            // Hien thi gia tri do am dat len Blynk

  // Mã nguồn xử lý chế độ Manual điều khiển thiết bị bằng tay
  if(FLAG_MANUAL==1){   //VAO CHE DO MANUAL
    analogWrite(QUAT,value);  // Đieu khien toc do quat
    Blynk.virtualWrite(V10, Toc_do_quat);  // Gửi giá trị tốc độ quạt lên điện thoại

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
    //FLAG_MANUAL=0;  
  }
}

// Mã nguồn xử lý điều khiển Triac
void ICACHE_RAM_ATTR TriacControl(){     //ICACHE_RAM_ATTR tra google
 // Serial.println("DA VAO CHUONG TRINH NGAT");
  if(FLAG_MODE==1){
    delayMicroseconds(Timer_2);   // sử dụng Timer_2 để test- kiểm tra PID dùng biến Timer_1 
    digitalWrite(Thanh_Nhiet, HIGH);
    delayMicroseconds(100);
    digitalWrite(Thanh_Nhiet, LOW);
   }
}

void setup()
{
    Serial.begin(115200);
    dht.begin();
    Blynk.begin(auth, ssid, pass,"iot.htpro.vn", 8080);     // Kết nối Mạng

    pinMode(INTERRUPT_PIN, INPUT_PULLUP);                   // CHÚ Ý 2 DÒNG NÀY_____Khai báo Ngắt
    attachInterrupt(INTERRUPT_PIN, TriacControl, FALLING);  // CHÚ Ý 2 DÒNG NÀY_____RISING- có 5 chế độ ngắt

    pinMode(LED_BUILTIN, OUTPUT);    
    pinMode(QUAT, OUTPUT);
    pinMode(BOM, OUTPUT);
    pinMode(Thanh_Nhiet, OUTPUT);     

    timer.setInterval(1000, sendUptime);  // Nhảy tới hàm sendUptime mỗi 1000ms (Co thể tăng thêm). Để tránh gửi dữ liệu liên tục lên Blynk Cloud.
}

void loop()
{
  Blynk.run();
  timer.run();

// Mã nguồn xử lý tính toán delay trước khi mở Triac
if(FLAG_PID_NHIET_DO==1){     //VAO CHE DO PID NHIET DO
  A = Nhietdodat - Temp_Sensor;

  if (A < 0) {
    Timer_1 = 0.9;
    }
    else{
      Alpha = 2*Time_sample*Kp + Ki*Time_sample*Time_sample + 2*Kd;
      Beta = Time_sample*Time_sample*Ki - 4*Kd - 2*Time_sample*Kp;
      Gama = 2*Kd;

      pid_Output = (Alpha*A + Beta*A1 + Gama*A2 + 2*Time_sample*pid_LastOutput) / (2*Time_sample);    // CÓ vẻ sai sai cần xem lại. test thử code chạy k ổn
      pid_LastOutput = pid_Output;             // Trong sách: U(k) = U(k-1) + (Kp+Kd/Ta)*e(k) + (Ki*Ta-Kp)*e(k-1) + Kd*e(k-2)/Ta;
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
    Sai_so = Temp_Sensor - Nhietdodat;
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
    digitalWrite(LED_BUILTIN, ledState);      

    Blynk.virtualWrite(V2, Timer_1);    // kiêm tra kết quả tính toán PID (xoá cũng được)
    
    if(Che_do==2){           // Tự động vào chế độ PID
      Serial.println("Day la che do MANUAL ");
       FLAG_MANUAL=1;
       FLAG_PID_NHIET_DO=0;
       FLAG_PID_QUAT=0;
       FLAG_MODE=0;
     }else{                   // Tự động vào chế độ PID
      Serial.println("Day la che do PID ");
       FLAG_MANUAL=0;
       FLAG_PID_QUAT=1;
       FLAG_PID_NHIET_DO=1;
       FLAG_MODE=1;
     }

  }
 
} // ket thuc void loop
