#include <Wire.h>  
#include "SSD1306Wire.h"
#include "DHT.h"

// inisialisasi pin OLED1306
#define ADDRESS 0x3c
#define SDA D1
#define SDC D2

// inisialisasi pin DHT
#define DHTPIN D3
#define DHTTYPE DHT22

// inisialisasi platform IoT Adafruit MQTT
#define IO_USERNAME   "fadhilc97"
#define IO_KEY        "9f3613ffe03148dd8761c9eeb0ea4104"
#define WIFI_SSID   "LxF"
#define WIFI_PASS   "79912192"
#include "AdafruitIO_WiFi.h"


DHT dht(DHTPIN, DHTTYPE);
SSD1306Wire display(0x3c, D1, D2);

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

AdafruitIO_Feed *suhu_udara = io.feed("suhu-udara");
AdafruitIO_Feed *kelembaban = io.feed("kelembaban");

void setup() {
  Serial.begin(115200);
  while(!Serial);

  io.connect();

  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println(io.statusText());

  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  dht.begin();
}

void loop() {
  io.run();
  display.clear();

  float hum = dht.readHumidity();
  float tempC = dht.readTemperature();
  float tempF = dht.readTemperature(true);

  if(isnan(hum) || isnan(tempC) || isnan(tempF)) {
    display.drawString(0, 0, "Failed to read from DHT sensor !");
    return;
  }

  suhu_udara->save(tempC);
  kelembaban->save(hum);

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 11, "Hum = " + String(hum) + " %");
  display.drawString(0, 22, "Temp = " + String(tempC) + " °C");
  display.drawString(0, 33, "Temp = " + String(tempF) + " °F");
  
  display.display();
  delay(4000);

}
