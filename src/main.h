#ifndef _MAIN_H
#define _MAIN_H


/* -------------------------------------------------------------------------- */
/*                                Include Files                               */
/* -------------------------------------------------------------------------- */
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <Preferences.h>

/* -------------------------------------------------------------------------- */
/*                                   Macros                                   */
/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */
/*                                Enumerations                                */
/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */
/*                                  Variables                                 */
/* -------------------------------------------------------------------------- */

/* ----------------------- MQTT client default values ----------------------- */
char mqtt_server[40];
char mqtt_port[6] = "8080";
char mqtt_user[34] = "";
char mqtt_password[34] = "";

/* ----------------------- MQTT WiFiManager Definition ---------------------- */
WiFiManager wm;
WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
WiFiManagerParameter custom_mqtt_port("port", "mqtt port", mqtt_port, 6);
WiFiManagerParameter custom_mqtt_user("mqtt_user", "MQTT Username", mqtt_user, 32);
WiFiManagerParameter custom_mqtt_password("mqtt_password", "MQTT Password", mqtt_password, 32);

/* -------------- Preference Object to store data in the memory ------------- */
Preferences preferences;  

/* -------------------------------------------------------------------------- */
/*                               Data Structures                              */
/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */
/*                                  Typedefs                                  */
/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */
/*                                  Functions                                 */
/* -------------------------------------------------------------------------- */
void saveConfigCallback () ;
void MQTT_paramInit();
void wm_init(bool _reset);
#endif