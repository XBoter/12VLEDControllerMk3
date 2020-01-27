
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
//---- Controller Defines
//#define DEV_CONTROLLER_MQTT_PATHS
#define DESK_CONTROLLER_MQTT_PATHS
//#define BED_CONTROLLER_MQTT_PATHS
//#define SHELF_CONTROLLER_MQTT_PATHS
//#define SHELF_DOWN_CONTROLLER_MQTT_PATHS
//#define TV_CONTROLLER_MQTT_PATHS
//#define KITCHEN_CONTROLLER_MQTT_PATHS
#include <secrets_mqtt.h>

//+++ Defines for Information Print +++//
//#define INFORMATION_MASTER_PRINT

//#define INFORMATION_WIFI_STATE
//#define INFORMATION_MQTT_STATE
//#define INFORMATION_HASSIO_STATE
//#define INFORMATION_MQTT_LED
//#define INFORMATION_MQTT_MOTION_DETECTION
//#define INFORMATION_MQTT_HASSIO
//#define INFORMATION_LIGHT_STATE
//#define INFORMATION_PIR_SENSOR

//-------------------- Basic Information --------------------//
#define Name        "LED-Strip-Controller-for-12V-Mk-4"
#define Programmer  "Nico Weidenfeller"
#define Created     "16.12.2019"
#define LastModifed "27.01.2020"
#define Version     "0.1.4"
/*
   Information    :  General Rework of Code from the Mk3 Software. PIR motion detection is now Interupt based.
                     Fixed WakeUp and Sleep routines. Added Alarm, noWiFi, noHassIO and noMqtt Effect. Removed Remote ESP restart Option.

   ToDo List      :  - Maybe add Warning when wrong mqtt commands (CW, WW or RGB) come in to inform the user of a wrong configuration
                     - Rework isRGB, isCW, isWW Check for MQTT, Network etc to reduce performance if nothing is connected or else
                     - Optimize MQTT for better performance with parameter for Strip 1 and 2 same for HassIO resend and Info tab
                     - Check ToDo marks
                     - Add Sync for Weekend mode so the effect doesnt desync
                     - Rework Resend and change if finished resend in LED Sleep and Wakeup

   Finished List  :  - Add Alarm Effect
                     - Add noMQTT Effect
                     - Add noWiFi Effect
                     - Add noHassIO Effect
                     - Struct for Settings
                     - Fix Color fade between diffrent effects
                     - Add Structs for MotionDetection and others
                     - Add Boundarie Check for json data
                     - Add State Machine for MQTT and WiFi connection
                     - Add PC Present motion stop
                     - Rework Motion Detection setup
                     - Add pir Motion detection with interrupts => Needes to be tested
                     - Fix WakeUp and Sleep routine

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
                     - Version 0.0.5
                         Started reworking led stuff (!Not compilable!)
                     - Version 0.1.0
                         Finished basic LED Control
                     - Version 0.1.1
                         Added Effects for NoHassIO, NoMQTT and NoWiFi. Fixed prio mode swap
                     - Version 0.1.2
                         Added Effects Alarm, Sleep, Wakeup and Weekend(not synced). Updatet ToDo list and added finished list. Added basic master present.
                     - Version 0.1.3
                         Fixed some bugs. Added motion detection effect. Added Only_RGB/CW/WW/RGBCW/RGBWW Effect/Mode. Fixed perma resend bug when HassIo is not aviable.
                     - Version 0.1.4
                         Added Option to disable Prio 3 LED Effects


*/


//+++ General Defines +++//
#define SETUP_BAUDRATE 115200

//---- LED STRIP 1 RGB PIN DEFINES
#ifdef STRIP_1_RGB
#define PIN_STRIP_1_RED D2
#define PIN_STRIP_1_GREEN D3
#define PIN_STRIP_1_BLUE D1
#endif

//---- LED STRIP 1 CW PIN DEFINE
#ifdef STRIP_1_CW
#define PIN_STRIP_1_COLD_WHITE D4   //ToDo
#endif

//---- LED STRIP 1 WW PIN DEFINE
#ifdef STRIP_1_WW
#define PIN_STRIP_1_WARM_WHITE NULL   //ToDo
#endif

//---- LED STRIP 2 RGB PIN DEFINES
#ifdef STRIP_2_RGB
#define PIN_STRIP_2_RED D6
#define PIN_STRIP_2_GREEN D7
#define PIN_STRIP_2_BLUE D5
#endif

//---- LED STRIP 2 CW PIN DEFINE
#ifdef STRIP_2_CW
#define PIN_STRIP_2_COLD_WHITE D8   //ToDo
#endif

//---- LED STRIP 2 WW PIN DEFINE
#ifdef STRIP_2_WW
#define PIN_STRIP_2_WARM_WHITE NULL   //ToDo
#endif

//---- MOTION DETECION PIN DEFINES
#ifdef CONTROLLER_MOTION_DETECTION
#define PIN_MOTION_SENSOR_1 D7
#define PIN_MOTION_SENSOR_2 D6
#endif


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

struct NetworkStatus {

  boolean Wifi_Connected    = false;
  boolean MQTT_Connected    = false;
  boolean HassIO_Connected  = false;

} NetworkState;


//*************************************************************************************************//
//----------------------------------------- MQTT Parameter ----------------------------------------//
//*************************************************************************************************//
//---- General
//-- HassIO
struct HassIOParameter {
  boolean SunBelowHorizon = false;
  uint8_t TimeHour        = 0;
  uint8_t TimeMin         = 0;
  boolean MasterPresent   = false;
  boolean PcPowerdOn      = false;
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
  e_None,
  e_Alarm,
  e_Wakeup,
  e_Sleep,
  e_Weekend,
  e_Only_RGB,
  e_Only_CW,
  e_Only_WW,
  e_Only_RGBCW,
  e_Only_RGBWW
};

//-- LED
struct StripParameter {
  boolean Power       = false;     // Power value       0 - 1
  uint8_t Red         = 0;         // Red value         0 - 255
  uint8_t Green       = 0;         // Green Value       0 - 255
  uint8_t Blue        = 0;         // Blue Value        0 - 255
  uint8_t Brightness  = 0;         // Brightness value  0 - 255
  uint8_t ColdWhite   = 0;         // Cold White value  0 - 255
  uint8_t WarmWhite   = 0;         // Warm White value  0 - 255
  EffectType Effect   = e_None;    // Effect
} ParameterLEDStrip1, ParameterLEDStrip2, InfoParameterLEDStrip1, InfoParameterLEDStrip2;

//-- Motion Detection
struct MotionDetectionParameter {
  boolean Power       = false;     // Power value on or off
  uint8_t Red         = 255;       // Color value from red
  uint8_t Green       = 64;        // Color value from green
  uint8_t Blue        = 0;         // Color value from blue
  uint16_t Timeout    = 20;        // Motion Timeout value (Sec)
} ParameterMotion, InfoParameterMotion;


//*************************************************************************************************//
//----------------------------------------------- LED ---------------------------------------------//
//*************************************************************************************************//

boolean DisablePrio3Effect = true;

const int MAX_COLOR_VALUE       = 255;
const int MIN_COLOR_VALUE       = 0;
const int MAX_BRIGHTNESS_VALUE  = 255;
const int MIN_BRIGHTNESS_VALUE  = 0;


//---------- LED STRIP DATA ----------//
struct StripData {

  //-- Brightness
  int Brightness  = 0;  //-- Brightness Value 0 - 255

  //-- RGB
  int R           = 0;  //-- Red Value   0 - 255
  int G           = 0;  //-- Green Value 0 - 255
  int B           = 0;  //-- Blue Value  0 - 255

  //-- CW
  int CW          = 0;  //-- Cold White Value 0 - 255

  //-- WW
  int WW          = 0;  //-- Warm White Value 0 - 255

} CurStrip1Data, CurStrip2Data, NextStrip1Data, NextStrip2Data;


//---------- FADE DATA ----------//
struct FadeData {

  //---- Fade
  //-- Parameter
  uint8_t FadeStepBrightness  = 5;  //-- Fade Step for Brightness 1 - 50
  uint8_t FadeStepColor       = 5;  //-- Fade Step for Color 1 - 50

  //-- Time
  const long TimeOutColorFade             = 10; //-- TimeOut for Color Fade
  const long TimeOutBrightnessFade        = 10; //-- TimeOut for Brightness Fade
  unsigned long PrevMillisColorFade       = 0;  //-- Previous Millis for Color Fade
  unsigned long PrevMillisBrightnessFade  = 0;  //-- Previous Millis for Brightness Fade

} FadeDataStrip1, FadeDataStrip2;


//---------- LED STRIP HARDWARE CONFIG ----------//
struct HardwareStripConfig {

  //-- RGB
  uint8_t PinRed        = 0xff;   //-- Pin for R channel 0 - 254
  uint8_t PinGreen      = 0xff;   //-- Pin for G channel 0 - 054
  uint8_t PinBlue       = 0xff;   //-- Pin for B channel 0 - 254
  boolean isRGB         = false;  //-- Strip support

  //-- CW
  uint8_t PinColdWhite  = 0xff;   //-- Pin for CW channel 0 - 254
  boolean isCW          = false;  //-- Strip support

  //-- WW
  uint8_t PinWarmWhite  = 0xff;   //-- Pin for WW channel 0 - 254
  boolean isWW          = false;  //-- Strip support

  //-- StripID
  uint8_t StripID       = 0xff;   //-- Strip ID 0 - 254
  boolean IdValid       = false;  //-- ID Validator

} HardwareConfigStrip1, HardwareConfigStrip2;

//---------- MODE LIST ----------//
enum StripMode {
  Idle,
  Normal,
  Motion,
  NoWiFi,
  NoMQTT,
  NoHassIO,
  Wakeup,
  Sleep,
  Weekend,
  Alarm
};

//---------- TRANSITION DATA ----------//
struct TransitionData {

  StripMode CurMode     = Idle;   //-- Init Mode Idle
  StripMode NextMode    = Idle;   //-- Init Mode Idle

  boolean Finished      = true;   //-- Is set true when the transition has finished
  uint8_t State         = 0;      //-- State of the transition
  int BackUpBrightness  = 0;      //-- Backup Brightness for NextData

} TransitionDataStrip1, TransitionDataStrip2, InfoTransitionDataStrip1, InfoTransitionDataStrip2;

//---------- BASIC EFFECT ----------//
struct BasicEffect {

  //-- Brightness
  int Brightness  = 0;

  //-- RGB
  int R   = 0;
  int G   = 0;
  int B   = 0;

  //-- CW
  int CW  = 0;

  //-- WW
  int WW  = 0;

  //-- Timer / Delay
  unsigned long PrevMillis    = 0;
  const unsigned long Timeout = 10;

  //-- State
  uint8_t State     = 0;
  uint8_t SubState  = 0;

  //-- Counter
  uint8_t Counter     = 0;
  uint8_t SubCounter  = 0;

};

//---------- EFFECT DATA LIST ----------//
struct EffectDataList {

  //---- Effect List
  //-- Prio 1
  BasicEffect NoHassIO;
  BasicEffect NoMQTT;
  BasicEffect NoWiFi;

  //-- Prio 2
  BasicEffect None;   //-- Includes mode Only_RGB/CW/WW/RGBCW/RGBWW
  BasicEffect Alarm;
  BasicEffect Wakeup;
  BasicEffect Sleep;
  BasicEffect Weekend;

  //-- Prio 3
  BasicEffect Motion;

} EffectDataListStrip1, EffectDataListStrip2;


struct MotionEffectData {

  boolean MotionDeteced       = false; //-- True when Motion is deteced by a Sensor
  boolean MotionEffectActive  = false;

} MotionData;

//*************************************************************************************************//
//---------------------------------------------- Motion -------------------------------------------//
//*************************************************************************************************//
struct PIRSensorData {

  volatile boolean MotionDeteced = false; //-- Gets True when a Sensor detects Motion

} PIRSensor1, PIRSensor2, InfoPIRSensor1, InfoPIRSensor2;


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
