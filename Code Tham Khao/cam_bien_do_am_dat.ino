#define pin_analog A0
#define pin_digital 3
#define pin_led 13

void setup() 
{
  Serial.begin(9600);//Mở cổng Serial ở mức 960
  pinMode (pin_analog, INPUT);
  pinMode(pin_digital, INPUT);
  pinMode (pin_led, OUTPUT);
}
 
void loop() 
{
  int do_am_dat = analogRead(pin_analog);     // Ta sẽ đọc giá trị hiệu điện thế của cảm biến
                                      // Giá trị được số hóa thành 1 số nguyên có giá trị
                                      // trong khoảng từ 0 đến 1024
                                      // Đất càng ẩm thì giá trị càng nhỏ. Đất khô thì giá trị lớn                                  
  Serial.println(do_am_dat);          //Xuất ra serial Monitor
  delay(1000);  
  // Đọc giá trị D0 rồi điều khiển Led 13...
  //Các bạn cũng có thể điều khiển bơm nước thông qua rơle...
  // Đất ẩm trả giá trị về 0 - Đất khô trả giá trị về 1
  if(digitalRead(pin_digital)==1)    // Đất khô
  {
    digitalWrite(pin_led,HIGH);// bật đèn LED cảnh báo
    // Làm thêm hành động bật máy bơm nước ở đây
    // ...
  }
  else // Đất ẩm
  {
    digitalWrite(pin_led, LOW);   // Tắt đèn LED
    // Tắt máy bơm nước
    // ...
  }
}
