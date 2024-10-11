#include "main.h"


void setup() {
    
    /* --------------------- Initialize Serial Communication -------------------- */
    Serial.begin(115200);
    
    /* -------------------------- Initialize Parameters ------------------------- */
    preferences.begin("my-app", false);

    /* ------------------------ Initiliaze WiFi Settings ------------------------ */
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP        
    
    /* ----------------------- Initialize MQTT Parameters ----------------------- */
    MQTT_paramInit();

    /* --------------------- Initialize WiFi Manager Object --------------------- */
    wm_init(true); 

}

void loop() {
  // put your main code here, to run repeatedly:
    wm.process();    
}


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

void saveConfigCallback () {
  /* --------------------------- Store MQTT settings -------------------------- */
  preferences.putString("mqtt_server",    custom_mqtt_server.getValue());
  preferences.putString("mqtt_port",      custom_mqtt_port.getValue());
  preferences.putString("mqtt_user",      custom_mqtt_user.getValue());
  preferences.putString("mqtt_password",  custom_mqtt_password.getValue());
}

void MQTT_paramInit(){

    /* ----------------------- Initialize MQTT Parameters ----------------------- */
    String _mqtt_port     = preferences.getString("mqtt_port",      mqtt_port);
    String _mqtt_server   = preferences.getString("mqtt_server",    mqtt_server);
    String _mqtt_user     = preferences.getString("mqtt_user",      mqtt_user);
    String _mqtt_password = preferences.getString("mqtt_password",  mqtt_password);
    
    custom_mqtt_server.setValue(_mqtt_server.c_str(),128);
    custom_mqtt_port.setValue(_mqtt_port.c_str(), 128);
    custom_mqtt_user.setValue(_mqtt_user.c_str(), 128);
    custom_mqtt_password.setValue(_mqtt_password.c_str(), 128);
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