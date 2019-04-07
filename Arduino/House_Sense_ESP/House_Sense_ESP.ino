#include <ESP8266WiFi.h>
#include <MQTT.h>

const char ssid[] = "jblap";
const char pass[] = "1234jblt";

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

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

  client.subscribe("esp/ledstrip");
  client.subscribe("helloListener");
  // client.unsubscribe("/hello");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
}

void setup() {
  Serial.begin(38700);
  WiFi.begin(ssid, pass);
  client.begin("broker.shiftr.io", net);
  client.onMessage(messageReceived);

  connect();
}

void loop() {
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }

  // publish a message roughly every second.
  if (millis() - lastMillis > 5000) {
    lastMillis = millis();
    client.publish("/phone", "world");
  }
}
