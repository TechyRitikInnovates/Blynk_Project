#define BLYNK_TEMPLATE_ID "TMPLjkS7yRsv"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "K5FKnIGgxUSzhc332cJMpwiyO7AGyN21"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>



const int lcdAddress = 0x27;  

// Set the LCD dimensions (16x2)
const int lcdColumns = 16;
const int lcdRows = 2;

LiquidCrystal_I2C lcd(lcdAddress, lcdColumns, lcdRows); 

char ssid[] = "Ritik";
char pass[] = "8291189618";

const int DHTPIN = 4;
const int ONE_WIRE_BUS = 23;
const int Fire = 19;
const int water = 16;

#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

void sendSensor() {
  int Fire_read = digitalRead(Fire);
  int water_read = digitalRead(water);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float sensorValue = 0.0;
  OneWire oneWire(ONE_WIRE_BUS);
  DallasTemperature sensors(&oneWire);
  sensors.requestTemperatures();
  sensorValue = sensors.getTempCByIndex(0);

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V1, h);
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V2, sensorValue);
  Blynk.virtualWrite(V3, 0);
  Blynk.virtualWrite(V4, 0);

  if (Fire_read == 0) {
    lcd.clear();
    Serial.println("Fire Detected");
    lcd.setCursor(0, 0);
    lcd.print("Fire Detected  ");
  } else if (water_read == 1) {
    lcd.clear();
    Serial.println("Water Detected");
    lcd.setCursor(0, 0);
    lcd.print("Water Detected ");
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DTemp: ");
    lcd.print(t);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("DHumidity: ");
    lcd.print(h);
    lcd.print("%");
    delay(3000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(sensorValue);
    lcd.print("C");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(Fire, INPUT);
  pinMode(water, INPUT);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  dht.begin();

  timer.setInterval(1000L, sendSensor);

  Wire.begin();
  lcd.init();      
  lcd.backlight(); 
}

void loop() {
  Blynk.run();
  timer.run();
}