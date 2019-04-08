//This is used because the FastLED library is unsure
//of what pin mapping the microcontroller is using if
//the microcontroller isnt an arduino model, also
//suppresses some warning messages that shouldn't show
#define FASTLED_ESP8266_RAW_PIN_ORDER 
#define CONFIG_LED_SPI_CHIPSET WS2801
#define FASTLED_INTERNAL
//Libraries to be included
#include <ESP8266WiFi.h>
#include <MQTT.h>
#include <ArduinoJson.h>
#include "DHT.h"
#include <SPI.h>
#include <FastLED.h>
//Definitions of pins and sensor types to be used by
// the program.
#define DHTPIN 4     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
#define DATA_PIN 0    //LED Strip Data
#define CLOCK_PIN 2   //LED Strip CLock
#define NUM_LEDS 5    //Number of LEDs in the LED Strip, to be increased as needed
//Wifi ssid and password for connecing to the Internet 
//and the MQTT Broker
const char ssid[] = "ssid";
const char pass[] = "ssidpass";
DHT dht(DHTPIN, DHTTYPE); //Initialises the DHT11 Sensor
WiFiClient net; // initialises the wifi
MQTTClient client; //initialises the MQTT client
CRGB leds[5]; //Initialises the LED strip
//Variables used throughout the program
unsigned long lastMillis = 0; //For use in debounce in conjuntion with millis()
String str;
int ledstripstate = 0;
int red = 0;
int blue = 0;
int green = 0;
const String roomname = "<ROOM>"; //Room name change per agent/node

void connect() { //Method for connecting to the WiFi and then the MQTT Broker
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) { //Waits until it is connected to the internet
    Serial.print(".");
    delay(1000);
  }
  Serial.print("\nconnecting...");
  while (!client.connect("<NODENAME>", "<USERNAME>", "<PASS>")) { //Node name, Username and Password for the MQTT Broker, replace as necessary
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nconnected!");   //Informs the user that the connection to the internet has been made
}

void messageReceived(String &topic, String &payload) { //handler for when a message is received from the MQTT Broker
//  Serial.println(payload); //Prints the entire message to the command line for testing purposes
  if(topic == "topicIn"){
    DynamicJsonBuffer recBuffer(1024);  //Creates a JsonBuffer to store al of the JSON string 
    JsonObject& obj = recBuffer.parseObject(payload);   //Creates a JSON Object from the json buffer that contains the payload received from the MQTT Broker
    red = obj["colour"][0]; // gets the RGB colours form the JSON Object
    green = obj["colour"][1];
    blue = obj["colour"][2];
    if(ledstripstate == 0){ //turns all of the LEDs the same colour if they were previously off
      for(int i = 0; i< NUM_LEDS; i++){
        leds[i].setRGB(red,green,blue);
      }
      ledstripstate = 1;
    } else { //turns all of the LEDs off if they were previously on
      for(int i = 0; i< NUM_LEDS; i++){
        leds[i].setRGB(0,0,0);
      }
      ledstripstate = 0;
    }
    FastLED.show(); //Updates the LED strip to match the new values
  }
}

void setup() {  //Setup
  Serial.begin(38700);    //Starts the serial monitor
  WiFi.begin(ssid, pass); //Starts the WiFi
  client.begin("broker.shiftr.io", net);  //Connects to the MQTT Broker
  client.onMessage(messageReceived);    //calls method when receives message from the broker
  dht.begin();  //Starts the DHT11 Sensor
  FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS); //Starts the program with the LEDs in an off state
    for(int i = 0; i< NUM_LEDS; i++){
    leds[i].setRGB(0,0,0);
  }
  FastLED.show(); //Updates the LED Strip
  connect(); //calls the method that connects the circuit to the wifi and the MQTT broker
  client.subscribe("topicIn"); //Subscribes the circuit to the topic "topicIn"
}

void loop() {
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) { //If the client disconnects, it reconnects and resubcribes to the topic
    connect();
    client.subscribe("topicIn");
  }

  // publish a message roughly every 10 seconds.
  if (millis() - lastMillis > 10000) {
    lastMillis = millis(); //sets it to the current time
    getSendTemp(); //calls the method to get the temperature and send it to the mqtt broker
  }
}
void getSendTemp(){ //Method for getting the readings from the sensors, serialises them and sends them to the MQTT Broker
  DynamicJsonBuffer jsonBuffer(1024); //creates a JSON Buffer
  float h = dht.readHumidity(); //reads the humidity from the sensor
  float t = dht.readTemperature(); //reads the temp from the sensor in Celcius as default because it is a sensible scale
  if (isnan(h) || isnan(t)) { //checks if there are readings being taken from the sensor, if it cant detect any, it tells the user
    Serial.println(F("Failed to read from DHT sensor!"));
    return; //restarts ther loop
  }
  float hic = dht.computeHeatIndex(t, h, false); // generates the heat index which is a more accurate version of the temp
  JsonObject& doc = jsonBuffer.createObject();  //creates an empty object from the buffer
  doc["room"] = roomname; //adds the room name to the JSON Object
  doc["ledState"] = ledstripstate; //adds the powered state to the JSON Object
  JsonArray& rgb = doc.createNestedArray("colour"); //creates a JSON array for storing the RGB colours for the LED strip
  rgb.add(red); //Add the values for RGB to the JSONArray
  rgb.add(green);
  rgb.add(blue);
  doc["temp"] = hic; //adds the heat index to the JSON Object
  doc["humidity"] = h;  //adds the humidity to the JSON Object
  String payload; //empty string
  doc.printTo(payload); //prints the buffer to the payload
  //Serial.println(payload); //for testing pruposes
  client.publish("topicOut", payload); //publishes the MQTT message
}
