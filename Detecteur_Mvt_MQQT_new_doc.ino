/*
  SimpleMQTTClient.ino
  The purpose of this exemple is to illustrate a simple handling of MQTT and Wifi connection.
  Once it connects successfully to a Wifi network and a MQTT broker, it subscribe to a topic and send a message to it.
  It will also send a message delayed 5 seconds later.
*/

#include "EspMQTTClient.h"

const char* ssid = "xxxxxxxx";
const char* password = "xxxxxxx";

#define SENSOR_PIN 12 // entrée du detecteur de mouvement 
//#define LED_PIN 13
//
//long lastMsg = 0;

EspMQTTClient client(
  ssid,
  password,
  "xxxxx",  // MQTT Broker server ip > xxxx à renseigner
  "xxxxx",   // Can be omitted if not needed > xxxx à renseigner
  "xxxxx",   // Can be omitted if not needed > xxxx à renseigner
  "xxxxx",     // Client name that uniquely identify your device> xxxx à renseigner
  1883              // The MQTT port, default to 1883. this line can be omitted
);

void setup()
{
  Serial.begin(9600);
  pinMode (SENSOR_PIN, INPUT);
  //  pinMode(LED_PIN, OUTPUT); // déclaration de la sortie LED

  // Optionnal functionnalities of EspMQTTClient : 
  client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
  client.enableHTTPWebUpdater(); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overrited with enableHTTPWebUpdater("user", "password").
  client.enableLastWillMessage("TestClient/lastwill", "I am going offline");  // You can activate the retain flag by setting the third parameter to true
}

// This function is called once everything is connected (Wifi and MQTT)
// WARNING : YOU MUST IMPLEMENT IT IF YOU USE EspMQTTClient
void onConnectionEstablished()
{

  // Subscribe to "mytopic/test" and display received message to Serial
  client.subscribe("mytopic/test", [](const String & payload) {
    Serial.println(payload);
  });

  // Subscribe to "mytopic/wildcardtest/#" and display received message to Serial
  client.subscribe("mytopic/wildcardtest/#", [](const String & topic, const String & payload) {
    Serial.println("(From wildcard) topic: " + topic + ", payload: " + payload);
  });

  // Publish a message to "mytopic/test"
  client.publish("mytopic/test", "This is a message"); // You can activate the retain flag by setting the third parameter to true

  // Execute delayed instructions
  client.executeDelayed(5 * 1000, []() {
    client.publish("mytopic/wildcardtest/test123", "This is a message sent 5 seconds later");
  });

}

void loop()
{
  client.loop();

  boolean state=digitalRead(SENSOR_PIN);
  if(state==HIGH) {
    Serial.println("Motion detected!");
    client.publish("Maison/ArriereCuisine/Mouvement", "Motion detected");
  }
  else {
    Serial.println("No Motion!");
  }
delay(1000); // Correction pour gagner en temps de réponse du système
}
