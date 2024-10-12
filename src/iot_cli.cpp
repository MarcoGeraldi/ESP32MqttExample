#include "iot_cli.h"

void controlLED(String params) {
  params.trim();
  if (params == "-on") {
    Serial.println("LED is now ON!");
    // Add code to turn on the LED here
  } else if (params == "-off") {
    Serial.println("LED is now OFF!");
    // Add code to turn off the LED here
  } else {
    Serial.println("Unknown parameter. Use '-on' or '-off'.");
  }
}

void sayHello(String params) {
  Serial.println("Hello, World!");
}

void reset(String params){
    Serial.println("resetting..");
    ESP.restart();
}

void mqtt_info(String params){
    Serial.println("MQTT Broker IP: "   + preferences.getString(PREF_MQTT_SERVER,   ""));
    Serial.println("MQTT Port: "        + preferences.getString(PREF_MQTT_PORT,     ""));
    Serial.println("MQTT Username: "    + preferences.getString(PREF_MQTT_USER,     ""));
    Serial.println("MQTT Password: "    + preferences.getString(PREF_MQTT_PASSWORD, ""));
}