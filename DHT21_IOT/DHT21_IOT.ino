
#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include <DHT.h>



char auth[] = "YourAuthToken";
BlynkTimer timer;
#define BLYNK_PRINT Serial

#define DHTPIN 2 
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHTPIN, DHTTYPE);


// Chương trình ngắt

// Đọc giá trị từ BLYNK

void sendSensor()
{
  float h = dht.readHumidity(); 
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
}

void setup()
{

  Serial.begin(9600);
  Blynk.begin(auth);
  dht.begin();


  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();

  // các điều kiện

}
