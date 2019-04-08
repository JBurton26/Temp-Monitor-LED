#define FASTLED_ESP8266_RAW_PIN_ORDER
#define CONFIG_LED_SPI_CHIPSET WS2801
#define FASTLED_INTERNAL

#include <ESP8266WiFi.h>
#include <MQTT.h>
#include <ArduinoJson.h>
#include "DHT.h"
#include <SPI.h>
#include <FastLED.h>

#define DHTPIN 4     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
#define DATA_PIN 0
#define CLOCK_PIN 2
#define NUM_LEDS 5

const char ssid[] = "jblap";
const char pass[] = "1234jblt";
DHT dht(DHTPIN, DHTTYPE); //Initialises the DHT11 Sensor
WiFiClient net;
MQTTClient client;
CRGB leds[5];

unsigned long lastMillis = 0;
String str;
int ledstripstate = 0;
int red = 0;
int blue = 0;
int green = 0;
const String roomname = "Kitchen";
void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("jbesp1", "cfc9ef9d", "aa8346a53372ae9f")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  // client.unsubscribe("/hello");
}

void messageReceived(String &topic, String &payload) {
  Serial.println(payload);
  if(topic == "esp/kitchen"){
    DynamicJsonBuffer recBuffer(1024);
    JsonObject& obj = recBuffer.parseObject(payload);
    red = obj["colour"][0];
    green = obj["colour"][1];
    blue = obj["colour"][2];
    for(int i = 0; i< NUM_LEDS; i++){
      leds[i].setRGB(red,green,blue);
    }
    FastLED.show();
  }
  
}

void setup() {
  Serial.begin(38700);
  WiFi.begin(ssid, pass);
  client.begin("broker.shiftr.io", net);
  client.onMessage(messageReceived);
  dht.begin();
  FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
    for(int i = 0; i< NUM_LEDS; i++){
    leds[i].setRGB(0,0,0);
  }
  FastLED.show();
  connect();
  client.subscribe("esp/kitchen");
  client.subscribe("helloListener");
}

void loop() {
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
    client.subscribe("esp/kitchen");
    client.subscribe("helloListener");
  }

  // publish a message roughly every second.
  if (millis() - lastMillis > 10000) {
    lastMillis = millis();
    getSendTemp();
  }
}
void getSendTemp(){
  DynamicJsonBuffer jsonBuffer(1024);
  float h = dht.readHumidity(); //reads the humidity from the sensor
  float t = dht.readTemperature(); //reads the temp from the sensor in Celcius as default because it is a sensible scale
  if (isnan(h) || isnan(t)) { //checks if there are readings being taken from the sensor, if it cant detect any, it tells the user
    Serial.println(F("Failed to read from DHT sensor!"));
    return; //restarts ther loop
  }
  float hic = dht.computeHeatIndex(t, h, false);
  JsonObject& doc = jsonBuffer.createObject();
  doc["room"] = roomname;
  doc["ledState"] = 0;
  JsonArray& rgb = doc.createNestedArray("colour");
  rgb.add(red);
  rgb.add(green);
  rgb.add(blue);
  doc["temp"] = hic;
  doc["humidity"] = h;
  String payload;
  doc.printTo(payload);
  //Serial.println(payload);
  client.publish("phone", payload);
}
