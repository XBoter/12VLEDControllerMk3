
//+++ Include Libarys +++//
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


//+++ Defines for WiFi Secrets +++//
#define WZ_WIFI_SETTINGS
//#define HDS_WIFI_SETTINGS
#include <secrets_wifi.h>

//++++ Defines for MQTT Secrets +++//
#define WZ_MQTT_SETTINGS
//#define HD_SMQTT_SETTINGS
#define MQTT_JSON_TEST
#include <secrets_mqtt.h>

//+++ Defines for Information Print +++//
#define INFORMATION_SYSTEM
#define INFORMATION_WIFI
#define INFORMATION_MQTT
#define INFORMATION_HASSIO
#define INFORMATION_MAIN_STATE
#define INFORMATION_LED_STRIP

//-------------------- Basic Information --------------------//
#define Name        "LED-Strip-Controller-for-12V-Mk-4"
#define Programmer  "Nico Weidenfeller"
#define Created     "16.12.2019"
#define LastModifed "18.12.2019"
#define Version     "0.0.3"
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
                     - Add Homeassistant color and brightness value update when a effect is going on


   Bugs           :  -

   Optimzie       :  -

   Patchhistory   :  - Version 0.0.1
                         Init Commit.
                     - Version 0.0.2
                         Started implementing network functions like mqtt and WiFi
                     - Version 0.0.3
                         Tryed Json out but didnt worked so it got removed. Rolled back to classic mqtt topics for each led parameter.


*/


//+++ General Defines +++//
#define SETUP_BAUDRATE 115200

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
uint8_t WiFiState = 0;
uint8_t MQTTState = 0;

//-- Variables
WiFiClient wifiMqtt;
PubSubClient mqttClient(wifiMqtt);
boolean StartWifi = true;
uint8_t NoWiFiCounter = 0;
uint16_t ESPHeartBeatCounter = 0;
boolean HassIOTimeout = false;


//*************************************************************************************************//
//----------------------------------------- MQTT Parameter ----------------------------------------//
//*************************************************************************************************//
//-- General
boolean SendMqttParameter = false;
char LastColorStrip1Holder[16];
char LastColorStrip2Holder[16];
char LastEffectStrip1Holder[16];
char LastEffectStrip2Holder[16];

//-- Specific
enum EffectType {
  None,
  Alarm,
  Wakeup,
  Sleep,
  Weekend
};

struct LEDParameter {
  boolean Power       = false;     // Power value on or off
  uint8_t Red         = 0;         // Color value from red
  uint8_t Green       = 0;         // Color value from green
  uint8_t Blue        = 0;         // Color value from blue
  uint8_t Brightness  = 0;         // Brightness value
  uint8_t White       = 0;         // White value
  EffectType Effect   = None;      // Effect
} FirstStrip, SecondStrip, InfoFirstStrip, InfoSecondStrip;



struct Settings {
  uint16_t MotionTimeout = 0;
  uint8_t MotionRedValue = 0;
  uint8_t MotionGreenValue = 0;
  uint8_t MotionBlueValue = 0;
  uint8_t MotionBrightness = 0;
  boolean UseMotionBrightness = false;
} MainSettings, InfoMainSettings;


//*************************************************************************************************//
//-------------------------------------------- LED Strip ------------------------------------------//
//*************************************************************************************************//
uint8_t StateStrip1 = 0;
uint8_t StateStrip2 = 0;

struct LEDStripData {
  double Brightness = 0;
  double White      = 0;
  double Red        = 0;
  double Green      = 0;
  double Blue       = 0;
} LEDStrip1, LEDStrip2, MotionStrip1, MotionStrip2;


//*************************************************************************************************//
//------------------------------------------- Information -----------------------------------------//
//*************************************************************************************************//
//---- Controller
uint8_t InformationMainState = 0;

//---- Network
boolean InformationOneTimePrintWiFiConnected = true;
boolean InformationOneTimePrintWiFiDisconnected = true;

boolean InformationOneTimePrintMQTTConnected = true;
boolean InformationOneTimePrintMQTTDisconnected = true;

boolean InformationOneTimePrintHassIOAvaibale = true;
boolean InformationOneTimePrintHassIONotAvaiable = true;

//---- MQTT Parameter
//-- System
boolean InformationMqttSystemReboot = false;

//-- General


//*************************************************************************************************//
//---------------------------------------------- Delay --------------------------------------------//
//*************************************************************************************************//

//Timer / Delay
unsigned long PrevMillisExample                    = 0;
unsigned long PrevMillisNoWiFiConnection           = 0;
unsigned long PrevMillisNoMQTTConnection           = 0;
unsigned long PrevMillisNoHassIOConnection         = 0;
unsigned long PrevMillisESPHeartBeat               = 0;
unsigned long PrevMillisLoop                       = 0;

unsigned long TimeOutExample                       = 1000;   // 1.00 Seconds
unsigned long TimeOutNoWiFiConnection              = 5000;   // 5.00 Seconds
unsigned long TimeOutNoMQTTConnection              = 5000;   // 5.00 Seconds
unsigned long TimeOutNoHassIOConnection            = 10000;  // 10.0 Seconds
unsigned long TimeOutESPHeartBeat                  = 300000; // 5.00 Minutes

/*
  unsigned long CurMillisExample = millis();
  if (CurMillisExample - PrevMillisExample >= TimeOutExample) {
    PrevMillisExample = CurMillisExample;

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
