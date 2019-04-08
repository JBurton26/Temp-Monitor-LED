#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include "DHT.h"
#include <SPI.h>
#include <FTRGBLED.h>
#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

int rgb[3];
String rgbString[3];
String str;
const int PIN_CKI = 6;
const int PIN_SDI = 7;

DHT dht(DHTPIN, DHTTYPE); //Initialises the DHT11 Sensor
SoftwareSerial mySerial(4,5); // RX,TX
RGBLEDChain led(5, PIN_CKI, PIN_SDI);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(38700);
  dht.begin();

  led.begin();
}

void loop() {
//  delay(5000); //wait 5 seconds between readings
  
//  float h = dht.readHumidity(); //reads the humidity from the sensor
//  float t = dht.readTemperature(); //reads the temp from the sensor in Celcius as default because it is a sensible scale
//  if (isnan(h) || isnan(t)) { //checks if there are readings being taken from the sensor, if it cant detect any, it tells the user
//    Serial.println(F("Failed to read from DHT sensor!"));
//    return; //restarts ther loop
//  }
//  float hic = dht.computeHeatIndex(t, h, false);
//  Serial.print(F("Humidity: "));
//  Serial.print(h);
//  Serial.print(F("%  Temperature: "));
//  Serial.print(hic);
//  Serial.println(F("°C "));
  DynamicJsonBuffer jsonBuffer;
  str = mySerial.readString();
  JsonObject& colourjson = jsonBuffer.parseObject(str);
  rgbString = colourjson[String("colour")];
  for(int i =0; i<rgbString.length; i++){
    rgb.add(rgbString[i]);
  }
  Serial.println(rgb[0]);
  led.setLEDs(rgb);
  led.update();
  delay(100);
}
