#include "DHT.h"
#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE); //Initialises the DHT11 Sensor

void setup() {
  Serial.begin(9600); //Starts the Serial on baud rate 9600 
  Serial.println(F("DHTxx test!"));
  dht.begin(); //starts the sensor
}

void loop() {
  delay(5000); //wait 5 seconds between readings
  float h = dht.readHumidity(); //reads the humidity from the sensor
  float t = dht.readTemperature(); //reads the temp from the sensor in Celcius as default because it is a sensible scale
  if (isnan(h) || isnan(t)) { //checks if there are readings being taken from the sensor, if it cant detect any, it tells the user
    Serial.println(F("Failed to read from DHT sensor!"));
    return; //restarts ther loop
  }
  float hic = dht.computeHeatIndex(t, h, false);
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(hic);
  Serial.println(F("°C "));
}
