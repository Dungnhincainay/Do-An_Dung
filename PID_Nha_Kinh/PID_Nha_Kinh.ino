/*
Code viet cho Esp8266 NodeMCU V3
https://bitly.com.vn/b6vwo7
GPIO 4, 5, 12, 13, 14, 16 có thể sử dụng bình thường.



Document: https://docs.blynk.cc/#blynk-firmware-virtual-pins-control
http://rancilio-pid.de/5-der-erste-test/

Máy chủ của bạn là: blynk.remoteapp.de hoặc iot.htpro.vn
Cổng: 9443 trên app và 8080 tren Esp

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


#define QUAT D1         //GPIO5
#define AC D2           //GPIO4 - thanh nhiet
float humidity_sensor=0;             // V4 - Do am khong khi
float temp_sensor=0;                // V5 - Nhiet do khong khi
float hum_ADC,ADC_VAL,temp_Set,sai_so;    // V6 - Do am Dat 

int flag=0;

 // V1 V2 : Chọn chế độ
 float value1;
 float Kp =0;      //Kp
 float Ki =0;      //Ki
 float Kd =0;      //Kd

int FLAG_PID,FLAG_MANUAL,FLAG_PID_NHIET_DO,BT_AC
int ledState = LOW;
unsigned long Ago = 0;
unsigned long Now = 0;
const long Set_time = 300;


BLYNK_CONNECTED() {
    Blynk.syncAll();
}
 BLYNK_WRITE(V1){
   value1 = param.asFloat(); // Get value as integer
 }
 BLYNK_WRITE(V7){   
   Kp = param.asFloat(); // Get value as integer
 }
 BLYNK_WRITE(V8){   
   Ki = param.asFloat(); // Get value as integer
 }
 BLYNK_WRITE(V9){   
   Kd = param.asFloat(); // Get value as integer
 }

void setup()
{
    Serial.begin(115200);
    Blynk.begin(auth, ssid, pass,"iot.htpro.vn", 8080);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(QUAT, OUTPUT);
    pinMode(AC, OUTPUT);
    timer.setInterval(2000, sendUptime);
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
 
 
  humidity_sensor = dht.readHumidity();         //doc gia tri do am tu  DHT21
  temp_sensor = dht.readTemperature();   // Doc gia tri nhiet do tu DHT21
  if (isnan(humidity_sensor) || isnan(temp_sensor)) {
      Serial.println("Loi khong co ket noi den DHT21!");
    }

  Serial.println("\nKet qua do duoc:");       // Hiển thị trên monitor
  Serial.print("Do Am = ");                     // Hiển thị trên monitor
  Serial.print(humidity_sensor);                              // Hiển thị trên monitor
  Serial.print("%  ");                          // Hiển thị trên monitor
  Serial.print("Nhiet do = ");                  // Hiển thị trên monitor
  Serial.print(temp_sensor);                              // Hiển thị trên monitor
  Serial.print("°C \n");                        // Hiển thị trên monitor
  Blynk.virtualWrite(V6, t);      // Gửi dữ liệu lên sever - lên Điện thoại.
  Blynk.virtualWrite(V5, h);      // Gửi dữ liệu lên sever - lên Điện thoại.
  
  if(t>=70 && flag==0){
    Blynk.notify("Nhiệt độ cao");
    flag=1;
  }else if(t<70){
      flag=0;
    }  
  ADC_VAL = analogRead(A0);           // Doc gia tri do am dat
  hum_ADC =((ADC_VAL/1024)/0.767442)*100;
  Blynk.virtualWrite(V6, hum_ADC);   // Hien thi gia tri do am dat len Blynk
}

void loop()
{
  Blynk.run();
  timer.run();
  //P1.2 Mã nguồn xử lý chế độ Manual điều khiển thiết bị bằng tay

  if(FLAG_MANUAL==1){///VAO CHE DO MANUAL
	  analogWrite(LED,DIM_Led); //bat Led
	  analogWrite(12,PWM_FAN);  //Bat quat
    if (bt2 == 1){
      digitalWrite(BOM,HIGH);//bat bom
	    Serial.println("BAT BOM");
    }else {
	    digitalWrite(BOM,LOW);//tat bom
	    Serial.println("TAT BOM");
    }
  
    if (BT_AC == 1){
      Serial.println("BAT THANH NHIET");
      digitalWrite(AC,HIGH);
    }else {
      Serial.println("TAT THANH NHIET");
	    digitalWrite(AC,LOW);
    }
	  FLAG_MANUAL=0;  
  }

  // Mã nguồn xử lý chế độ điều khiển cho Quạt
  if(FLAG_PID==1){  ////VAO CHE DO TU DONG CHO QUAT
    Serial.println("Bat dau CHAY TU DONG CHO QUAT");
    Serial.println(nhietdodat);
    sai_so = temp_sensor - temp_Set;
    if (sai_so < 0) {
       analogWrite(QUAT, 0);
       Serial.println("analogWrite(QUAT, 0)");
    }
    else 
      if (sai_so >= 5){
        analogWrite(QUAT, 1024);
        Serial.println("analogWrite(QUAT, 1024)");
      }
      else 
        if (sai_so >= 3 && E < 5){
          analogWrite(QUAT, 900);
          Serial.println("analogWrite(QUAT, 900)");
        }
        else 
          if (sai_so >= 1 && sai_so < 3){
          analogWrite(QUAT, 800);
          Serial.println("analogWrite(QUAT, 800)");
          }
          else {
            analogWrite(QUAT, 550);
            Serial.println("analogWrite(QUAT, 550)");
          }
    FLAG_PID=0;
  }//Ket thuc ham chay tu dong cho quat
} // void loop

//___P1.6 Mã nguồn xử lý điều khiển Triac
void ICACHE_RAM_ATTR TriacControl(){ //them ICACHE_RAM_ATTR
  Serial.println("DA VAO CHUONG TRINH NGAT");
  if(FLAG_MODE==1){
    delayMicroseconds(timer_1 * 10000);
    digitalWrite(AC, HIGH);
    delayMicroseconds(100);
   digitalWrite(AC, LOW);
   }
}