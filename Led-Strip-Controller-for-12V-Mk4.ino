
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
#define DEV_CONTROLLER_MQTT_PATHS
#include <secrets_mqtt.h>

//+++ Defines for Information Print +++//
#define INFORMATION_WIFI_STATE
#define INFORMATION_MQTT_STATE
#define INFORMATION_HASSIO_STATE
#define INFORMATION_MQTT_LED
#define INFORMATION_MQTT_MOTION_DETECTION
#define INFORMATION_MQTT_HASSIO
#define INFORMATION_MAIN_STATE

//-------------------- Basic Information --------------------//
#define Name        "LED-Strip-Controller-for-12V-Mk-4"
#define Programmer  "Nico Weidenfeller"
#define Created     "16.12.2019"
#define LastModifed "30.12.2019"
#define Version     "0.0.4"
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
                     - Add json Data input over MQTT  (Not Used anymore)
                     - Add pir Motion detection with interrupts
                     - Add Structs for MotionDetection and others
                     - Add Boundarie Check for json data
                     - Add Option to turn of Motion detection Britghness Control
                     - Add State Machine for MQTT and WiFi connection
                     - Add Homeassistant color and brightness value update when a effect is going on
                     - Check ToDo marks


   Bugs           :  -

   Optimzie       :  -

   Patchhistory   :  - Version 0.0.1
                         Init Commit.
                     - Version 0.0.2
                         Started implementing network functions like mqtt and WiFi
                     - Version 0.0.3
                         Tryed Json out but didnt worked so it got removed. Rolled back to classic mqtt topics for each led parameter.
                     - Version 0.0.4
                         Added led fade, hassio mqtt and cleaned code a bit. added bound check for some mqtt parameter. Starte code implementation of new LED control


*/


//+++ General Defines +++//
#define SETUP_BAUDRATE 115200

//---- LED STRIP 1 RGB PIN DEFINES
#ifdef CONTROLLER_RGB or CONTROLLER_RGBW
#define PIN_STRIP_1_RED D2
#define PIN_STRIP_1_GREEN D3
#define PIN_STRIP_1_BLUE D1
#endif

//---- LED STRIP 1 W PIN DEFINES
#ifdef CONTROLLER_RGBW
#define PIN_STRIP_1_WHITE ToDo
#endif

//---- LED STRIP 2 RGB PIN DEFINES
#ifdef CONTROLLER_RGB or CONTROLLER_RGBW
#define PIN_STRIP_2_RED D6
#define PIN_STRIP_2_GREEN D7
#define PIN_STRIP_2_BLUE D5
#endif

//---- LED STRIP 2 W PIN DEFINES
#ifdef CONTROLLER_RGBW
#define PIN_STRIP_2_WHITE ToDo
#endif

//---- MOTION DETECION PIN DEFINES
#ifdef CONTROLLER_MOTION_DETECTION
#define PIN_MOTION_SENSOR_1 D7
#define PIN_MOTION_SENSOR_2 D6
#endif


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
//---- General
//-- HassIO
struct HassIOParameter {
  boolean SunBelowHorizon = false;
  uint8_t TimeHour        = 0;
  uint8_t TimeMin         = 0;
} ParameterHassIO, InfoParameterHassIO;

//-- Motion
boolean SendMqttParameter = false;
char LastColorStrip1Holder[16];
char LastColorStrip2Holder[16];
char LastEffectStrip1Holder[16];
char LastEffectStrip2Holder[16];

//---- Specific
//-- Effect List
enum EffectType {
  None,
  Alarm,
  Wakeup,
  Sleep,
  Weekend
};

//-- LED
struct LEDParameter {
  boolean Power       = false;     // Power value on or off
  uint8_t Red         = 0;         // Color value from red
  uint8_t Green       = 0;         // Color value from green
  uint8_t Blue        = 0;         // Color value from blue
  uint8_t Brightness  = 0;         // Brightness value
  uint8_t White       = 0;         // White value
  EffectType Effect   = None;      // Effect
} ParameterLEDStrip1, ParameterLEDStrip2, InfoParameterLEDStrip1, InfoParameterLEDStrip2;

//-- Motion Detection
struct MotionDetectionParameter {
  boolean Power       = false;     // Power value on or off
  uint8_t Red         = 0;         // Color value from red
  uint8_t Green       = 0;         // Color value from green
  uint8_t Blue        = 0;         // Color value from blue
  uint16_t Timeout    = 0;         // Motion Timeout value
} ParameterMotion, InfoParameterMotion;


//*************************************************************************************************//
//----------------------------------------------- LED ---------------------------------------------//
//*************************************************************************************************//
//-- Transition
boolean TransStrip1Finished = false;
uint8_t StateTransition = 0;


//-- Fade
const int FadeStepSize = 5;
const uint8_t FadeSpeedColorStrip1 = 20;
const uint8_t FadeSpeedColorStrip2 = 20;
const uint8_t FadeSpeedBrightnessStrip1 = 20;
const uint8_t FadeSpeedBrightnessStrip2 = 20;

//-- General
struct LEDStripData {
  int Brightness = 0;
  int White      = 0;
  int Red        = 0;
  int Green      = 0;
  int Blue       = 0;
} CurLEDStrip1, CurLEDStrip2, NextLEDStrip1, NextLEDStrip2;


//-- Mode Structs


//-- Mode List
enum LEDModes {
  Idle_Mode,
  Normal_Mode,
  Motion_Mode,
  NoWiFi_Mode,
  NoMQTT_Mode,
  NoHassIO_Mode,
  GoodMorning_Mode,
  GoodNight_Mode,
  Weekend_Mode,
  Alarm_Mode
} CurLEDStrip1Mode, NextLEDStrip1Mode, CurLEDStrip2Mode, NextLEDStrip2Mode;

//*************************************************************************************************//
//---------------------------------------------- Motion -------------------------------------------//
//*************************************************************************************************//
struct MotionData {
  volatile boolean MotionDeteced = false;
  double Brightness = 0;
  double Red        = 0;
  double Green      = 0;
  double Blue       = 0;
} PIRSensor1, PIRSensor2;


//*************************************************************************************************//
//------------------------------------------- Information -----------------------------------------//
//*************************************************************************************************//
//---- Controller
uint8_t InformationMainState = 0;

//---- Network
boolean InformationOneTimePrintWiFiConnected      = true;
boolean InformationOneTimePrintWiFiDisconnected   = true;

boolean InformationOneTimePrintMQTTConnected      = true;
boolean InformationOneTimePrintMQTTDisconnected   = true;

boolean InformationOneTimePrintHassIOAvaibale     = true;
boolean InformationOneTimePrintHassIONotAvaiable  = true;

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
unsigned long PrevMillisFadeColorStrip1            = 0;
unsigned long PrevMillisFadeColorStrip2            = 0;
unsigned long PrevMillisFadeBrightnessStrip1       = 0;
unsigned long PrevMillisFadeBrightnessStrip2       = 0;
unsigned long PrevMillisTransitionSpeedStrip1      = 0;
unsigned long PrevMillisTransitionSpeedStrip2      = 0;

unsigned long TimeOutExample                       = 1000;    // 1.00 Seconds
unsigned long TimeOutNoWiFiConnection              = 5000;    // 5.00 Seconds
unsigned long TimeOutNoMQTTConnection              = 5000;    // 5.00 Seconds
unsigned long TimeOutNoHassIOConnection            = 10000;   // 10.0 Seconds
unsigned long TimeOutESPHeartBeat                  = 300000;  // 5.00 Minutes

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
