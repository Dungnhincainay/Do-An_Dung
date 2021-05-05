
//P1.1: Mã nguồn xử lý tính toán delay trước khi mở Triac
  if(FLAG_PID_NHIET_DO==1){///VAO CHE DO PID NHIET DO
  A = nhietdodat - sensorTemp;
  Serial.println("Day la che do PID");
  Serial.println(led_Kp);
  Serial.println(led_Ki);
  Serial.println(led_Kd);
    if (A < 0) {
    timer_1 = 0.9;
  }
  else{
  led_alpha = 2*led_T*led_Kp + led_Ki*led_T*led_T + 2*led_Kd;
  led_beta = led_T*led_T*led_Ki - 4*led_Kd - 2*led_T*led_Kp;
  led_gama = 2*led_Kd;
  
  led_Output = (led_alpha*A + led_beta*A1 + led_gama*A2 +  2*led_T*led_LastOutput) / (2*led_T);
  led_LastOutput = led_Output;
  A2=A1;
  A1=A;
  
  if (led_Output>255){
    led_Output = 255;
  }
  else{
  if (led_Output<1){
    led_Output = 1;
  }
  }
  timer_1 = 1/led_Output;
   if (timer_1<0){
    timer_1 = 0;
  }
  else{
  if (timer_1>0.9){
    timer_1 = 0.9;
  }
  }
}	  
	FLAG_PID_NHIET_DO=0;  
  }

//P1.2 Mã nguồn xử lý chế độ Manual điều khiển thiết bị bằng tay
 if(FLAG_MANUAL==1){///VAO CHE DO MANUAL

	analogWrite(12,PWM_FAN);
  if (bt2 == 1){
    digitalWrite(BOM,HIGH);//bat bom
	Serial.println("BAT BOM");
  }
  else {
	digitalWrite(BOM,LOW);//tat bom
	Serial.println("TAT BOM");
  }
    if (BT_AC == 1){
    Serial.println("BAT THANH NHIET");
    digitalWrite(AC,HIGH);
  }
  else {
    Serial.println("TAT THANH NHIET");
	digitalWrite(AC,LOW);
  }
	FLAG_MANUAL=0;  
  }

//P1.3 Mã nguồn xử lý chế độ điều khiển cho Quạt

  if(FLAG_PID==1){////VAO CHE DO TU DONG CHO QUAT
  Serial.println("Bat dau CHAY TU DONG CHO QUAT");
  Serial.println(nhietdodat);
  E = sensorTemp - nhietdodat;
  if (E < 0) {
     analogWrite(QUAT, 0);
     Serial.println("analogWrite(QUAT, 0)");
  }
  else if (E >= 5){
    analogWrite(QUAT, 1024);
    Serial.println("analogWrite(QUAT, 1024)");
  }
  else if (E >= 3 && E < 5){
    analogWrite(QUAT, 900);
    Serial.println("analogWrite(QUAT, 900)");
  }
  else if (E >= 1 && E < 3){
    analogWrite(QUAT, 800);
    Serial.println("analogWrite(QUAT, 800)");
  }
  else {
    analogWrite(QUAT, 550);
    Serial.println("analogWrite(QUAT, 550)");
  }
  FLAG_PID=0;
  }//Ket thuc ham chay tu dong cho quat



//P1.5 Mã nguồn xử lý chế độ đo nhiệt độ, độ ẩm không khí, độ ẩm đất
void sendSensor(){//doc nhiet do do am khong khi
   float h = dht.readHumidity();
  sensorTemp = dht.readTemperature();
  Blynk.virtualWrite(V4, sensorTemp);
  Serial.println("Nhiet do la  ");
  Serial.print(sensorTemp);
  Serial.println("Do am la  ");
	Blynk.virtualWrite(V15, h);
  Serial.println(h);
    if (isnan(h) || isnan(sensorTemp)) {
    Serial.println("Loi khong co ket noi den DHT21!");
                                        }
    if(sensorTemp>40){
    Blynk.notify("Hey,HUY! Nhiet do qua nguong 40 do");
	}

  ADC_VAL = analogRead(A0);
  doamdat =((ADC_VAL/1024)/0.767442)*100;
  Blynk.virtualWrite(V16, doamdat);
}
//P1.6 Mã nguồn xử lý điều khiển Triac
void ICACHE_RAM_ATTR TriacControl(){ //them ICACHE_RAM_ATTR
  Serial.println("DA VAO CHUONG TRINH NGAT");
   if(FLAG_MODE==1){
    delayMicroseconds(timer_1 * 10000);
    digitalWrite(AC, HIGH);
    delayMicroseconds(100);
   digitalWrite(AC, LOW);
   }
}
