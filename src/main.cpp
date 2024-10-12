#include "main.h"


// Create the list of commands
tinyCLI::Command commands[] = {    
    {"help",            "Shows this help message"       , nullptr},  // Placeholder for help function
    {"reset",           "Reset the device"              , reset},
    {"mqtt_info",       "Prints stored mqtt settings"   , mqtt_info},
    {"mqtt_server",     "Set MQTT broker IP address"    , nullptr},
    {"mqtt_port",       "Set MQTT broker port"          , nullptr},
    {"mqtt_user",       "Set MQTT username"             , nullptr},
    {"mqtt_password",   "Set MQTT username"             , nullptr},   
    {"config_portal",   "Set Config Portal"             , nullptr},
    {"wifi_ssid",       "Set WiFi SSID"                 , nullptr},
    {"wifi_pswd",       "Set WiFi Password"             , nullptr},

};

// Initialize the command line interface
tinyCLI commandLine(Serial, commands, sizeof(commands) / sizeof(commands[0]));


void setup() {
    
    /* --------------------- Initialize Serial Communication -------------------- */
    Serial.begin(115200);
    pinMode(LED, OUTPUT);
    
    /* -------------------------- Initialize Parameters ------------------------- */
    preferences.begin("my-app", false);

    /* ------------------------ Initiliaze WiFi Settings ------------------------ */
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP        
    
    /* ----------------------- Initialize MQTT Parameters ----------------------- */
    MQTT_paramInit();

    /* --------------------- Initialize WiFi Manager Object --------------------- */
    wm_init(false); 
}

void loop() {
  /* ------------------------- avoid overflow of timer ------------------------ */
  if (timestamp > millis()) timestamp = 0;// put your main code here, to run repeatedly:
  
  /* ----------------------- Process Command line inputs ---------------------- */
  commandLine.processInput();

  /* -------------------------- WiFi manager Process -------------------------- */
  wm.process();    

  /* ------------------------- Connect to MQTT Broker ------------------------- */
  if (WL_CONNECTED == WiFi.status()){
    /* ------------------------------ MQTT Process ------------------------------ */
    if (!mqttClient.connected()){
      if ( millis() - timestamp  > 5000) {
          MQTT_reconnect();   //Try to reconnect each 5s 
          timestamp = millis();
        }
    } else {
      mqttClient.loop();
      mqttClient.publish("esp32/temperature", "19°C");
    } 
  }  
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

// Example command functions with parameters
void saveConfigCallback () {
  /* --------------------------- Store MQTT settings -------------------------- */
  preferences.putString(PREF_MQTT_SERVER,    custom_mqtt_server.getValue());
  preferences.putString(PREF_MQTT_PORT,      custom_mqtt_port.getValue());
  preferences.putString(PREF_MQTT_USER,      custom_mqtt_user.getValue());
  preferences.putString(PREF_MQTT_PASSWORD,  custom_mqtt_password.getValue());
}

void MQTT_paramInit(){

    /* ----------------------- Initialize MQTT Parameters ----------------------- */
    String _mqtt_port     = preferences.getString(PREF_MQTT_PORT,      mqtt_port);
    String _mqtt_server   = preferences.getString(PREF_MQTT_SERVER,    mqtt_server);
    String _mqtt_user     = preferences.getString(PREF_MQTT_USER,      mqtt_user);
    String _mqtt_password = preferences.getString(PREF_MQTT_PASSWORD,  mqtt_password);
    
    custom_mqtt_server.setValue(_mqtt_server.c_str(),128);
    custom_mqtt_port.setValue(_mqtt_port.c_str(), 128);
    custom_mqtt_user.setValue(_mqtt_user.c_str(), 128);
    custom_mqtt_password.setValue(_mqtt_password.c_str(), 128);
  
    mqttClient.setServer(custom_mqtt_server.getValue(), 1883);
    mqttClient.setCallback(MQTT_callback);
}

void wm_init(bool _reset){
    
    if (_reset) wm.resetSettings(); //reset settings - wipe credentials for testing
    
    wm.addParameter(&custom_mqtt_server);
    wm.addParameter(&custom_mqtt_port);
    wm.addParameter(&custom_mqtt_user);
    wm.addParameter(&custom_mqtt_password);

    wm.setConfigPortalBlocking(false);
    wm.setConfigPortalTimeout(60);

    //automatically connect using saved credentials if they exist
    //If connection fails it starts an access point with the specified name
    if(wm.autoConnect("AutoConnectAP")){
        Serial.println("connected...yeey :)");
    }
    else {
      Serial.println("Failed to connect");
    }
    
    //set config save notify callback
    wm.setSaveParamsCallback(saveConfigCallback);
}

void MQTT_callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "esp32/output") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(LED, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(LED, LOW);
    }
  }
}

void MQTT_reconnect() {
  
  /* ------------------------ Retrieve data from memory ----------------------- */
  String _mqtt_user     = preferences.getString(PREF_MQTT_USER,      mqtt_user);
  String _mqtt_password = preferences.getString(PREF_MQTT_PASSWORD,  mqtt_password);
  String _mqtt_port     = preferences.getString(PREF_MQTT_PORT,      mqtt_port);
  String _mqtt_server   = preferences.getString(PREF_MQTT_SERVER,    mqtt_server);
  
  /* ---------------------- Remove whitespaces at the end --------------------- */
  _mqtt_user.trim();
  _mqtt_password.trim();
  _mqtt_port.trim();
  _mqtt_server.trim();

  if (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    mqttClient.setServer(_mqtt_server.c_str(), _mqtt_port.toInt());
      
    if (mqttClient.connect("ESP8266Client99", _mqtt_user.c_str(), _mqtt_password.c_str())) {
      Serial.println("connected");
      // Subscribe
      mqttClient.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
    }
  }
}
