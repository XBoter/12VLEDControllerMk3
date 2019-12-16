
//+++ Include Libarys +++//
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//+++ Defines for WiFi Secrets +++//
#define WZ_WIFI_SETTINGS
//#define HDS_WIFI_SETTINGS
#include <secrets_wifi.h>

//++++ Defines for MQTT Secrets +++//
#define WZ_MQTT_SETTINGS
//#define HDS_MQTT_SETTINGS
#define MQTT_JSON_TEST
#include <secrets_mqtt.h>

//+++ Defines for Information Print +++//
#define INFORMATION_SYSTEM
#define INFORMATION_WIFI
#define INFORMATION_MQTT
#define INFORMATION_MAIN_STATE

//-------------------- Basic Information --------------------//
#define Name        "LED-Strip-Controller-for-12V-Mk-4"
#define Programmer  "Nico Weidenfeller"
#define Created     "16.12.2019"
#define LastModifed "16.12.2019"
#define Version     "0.0.2"
/*
   Information    :  General Rework of Code from the Mk3 Software. Changed Data send from Homeassistant to json. PIR motion detection is no Interupt based.
                     Fixed WakeUp and Sleep routines. Added Alarm, noWiFi, noHassIO and noMqtt Effect. Removed Remote ESP restart Option.

   ToDo List      :  - Add Alarm Effect
                     - Add noMQTT Effect
                     - Add noWiFi Effect
                     - Add noHassIO Effect
                     - Struct for Settings
                     - Fix WakeUp and Sleep routine
                     - Fix Color fade between diffrent effects
                     - Add json Data input over MQTT
                     - Add pir Motion detection with interrupts
                     - Add Structs for MotionDetection and others
                     - Add Boundarie Check for json data
                     - Add Option to turn of Motion detection Britghness Control
                     - Add State Machine for MQTT and WiFi connection
                     - Add Legacy Option to work with the old Controller boards
                     - Add Homeassistant color and brightness value update when a effect is going on

                     !!!! Final Check code for right variable names !!!!


   Bugs           :  -

   Optimzie       :  -

   Patchhistory   :  - Version 0.0.1
                         Init Commit.
                     - Version 0.0.2
                         Started implementing network functions like mqtt and WiFi 



*/


//+++ General Defines +++//
#define SETUP_BAUD_RATE 115200

#define PIN_STRIP_1_RED D1
#define PIN_STRIP_1_GREEN D2
#define PIN_STRIP_1_BLUE D3
#define PIN_STRIP_1_WHITE D4

#define PIN_STRIP_2_RED D5
#define PIN_STRIP_2_GREEN D6
#define PIN_STRIP_2_BLUE D7
#define PIN_STRIP_2_WHITE D8

//*************************************************************************************************//
//------------------------------------------- Controller ------------------------------------------//
//*************************************************************************************************//


//*************************************************************************************************//
//--------------------------------------------- Network -------------------------------------------//
//*************************************************************************************************//
//-- State
uint8_t WiFi_State = 0;
uint8_t MQTT_State = 0;

//-- Variables
WiFiClient wifi_Mqtt;
PubSubClient mqtt_Client(wifi_Mqtt);
boolean Start_Wifi = true;
uint8_t No_WiFi_Counter = 0;
uint16_t ESP_Heart_Beat_Counter = 0;

boolean OneTimePrintWiFiConnected = false;
boolean OneTimePrintWiFiDisconnected = false;


//*************************************************************************************************//
//----------------------------------------- MQTT Parameter ----------------------------------------//
//*************************************************************************************************//
//-- System
boolean mqtt_System_Reboot = false;

//-- General
struct Homeassistant_LED_Parameter {
  boolean Power = false;          // Power value on or off
  uint8_t Red_Value = 0;          // Color value from red
  uint8_t Green_Value = 0;        // Color value from green
  uint8_t Blue_Value = 0;         // Color value from blue
  uint8_t Brightness_Value = 0;   // Brightness value
  uint8_t White_Value = 0;        // White value
  uint8_t Effect_Number = 0;       // Effect number
} Main_Strip, Second_Strip, Info_Main_Strip, Info_Second_Strip;

struct Homeassistant_Settings {
  uint16_t Motion_Timeout = 0;
  uint8_t Motion_Red_Value = 0;
  uint8_t Motion_Green_Value = 0;
  uint8_t Motion_Blue_Value = 0;
  uint8_t Motion_Brightness = 0;
  boolean Use_Motion_Brightness = false;
} Main_Settings, Info_Main_Settings;


//*************************************************************************************************//
//-------------------------------------------- LED Strip ------------------------------------------//
//*************************************************************************************************//
struct LED_Strip_Data {
  uint8_t Red = 0;
  uint8_t Green = 0;
  uint8_t Blue = 0;
  uint8_t Brightness = 0;
  uint8_t White = 0;
} LED_Strip_1, LED_Strip_2, Motion_Strip_1, Motion_Strip_2;

//*************************************************************************************************//
//------------------------------------------- Information -----------------------------------------//
//*************************************************************************************************//
//---- Controller
uint8_t Information_MainState = 0;

//---- Network
boolean One_Time_Print_WiFi_Connected = true;
boolean One_Time_Print_WiFi_Disconnected = true;

//---- MQTT Parameter
//-- System
boolean Information_Mqtt_System_Reboot = false;

//-- General


//*************************************************************************************************//
//---------------------------------------------- Delay --------------------------------------------//
//*************************************************************************************************//

//Timer / Delay
unsigned long PrevMillis_Example                    = 0;
unsigned long PrevMillis_No_WiFi_Connection         = 0;
unsigned long PrevMillis_No_MQTT_Connection         = 0;
unsigned long PrevMillis_HeartBeat                  = 0;

unsigned long TimeOut_Example                       = 1000;   // 1.00 Seconds
unsigned long TimeOut_No_WiFi_Connection            = 5000;   // 5.00 Seconds
unsigned long TimeOut_No_MQTT_Connection            = 5000;   // 5.00 Seconds
unsigned long TimeOut_HeartBeat                     = 300000; // 5.00 Minutes

/*
  unsigned long CurMillis_Example = millis();
  if (CurMillis_Example - PrevMillis_Example >= TimeOut_Example) {
    PrevMillis_Example = CurMillis_Example;

  }
*/


//*************************************************************************************************//
//------------------------------------------ Setup and Loop ---------------------------------------//
//*************************************************************************************************//
//-- Used in Controller.ino --//
void setup() {
  SetupController();
}

void loop() {
  LoopController();
}
