

void SetupController() {
  Serial.begin(SETUP_BAUDRATE);
  Serial.println("");
  Serial.println("");
  delay(100);

  Serial.print("Name          : ");
  Serial.println(Name);
  Serial.print("Programmer    : ");
  Serial.println(Programmer);
  Serial.print("Created       : ");
  Serial.println(Created);
  Serial.print("Last Modifed  : ");
  Serial.println(LastModifed);
  Serial.print("Version       : ");
  Serial.println(Version);
  Serial.println("");

  Serial.println("Start Setup");

  Serial.println("  Set MQTT Parameter");
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  mqttClient.setCallback(callback);
  Serial.println("  Finished setting MQTT Parameter");

  //---- Init Variables
  PrevMillisNoHassIOConnection = millis();

  Serial.println("  Start PINs Initialization");

  //---------------- LED Strip 1 ----------------//
  if (CONTROLLER_NUM_LED_STRIPS >= 1) {

    //-- Set Hardware Info
    HardwareConfigStrip1.StripID = 1;
    HardwareConfigStrip1.IdValid = true;

    //-- Set Supported Modes
#ifdef STRIP_1_RGB
    HardwareConfigStrip1.isRGB = true;
    pinMode(PIN_STRIP_1_RED,    OUTPUT);
    pinMode(PIN_STRIP_1_GREEN,  OUTPUT);
    pinMode(PIN_STRIP_1_BLUE,   OUTPUT);
    HardwareConfigStrip1.PinRed       = PIN_STRIP_1_RED;
    HardwareConfigStrip1.PinGreen     = PIN_STRIP_1_GREEN;
    HardwareConfigStrip1.PinBlue      = PIN_STRIP_1_BLUE;
#endif

#ifdef STRIP_1_CW
    HardwareConfigStrip1.isCW = true;
    pinMode(PIN_STRIP_1_COLD_WHITE, OUTPUT);
    HardwareConfigStrip1.PinColdWhite = PIN_STRIP_1_COLD_WHITE;
#endif

#ifdef STRIP_1_WW
    HardwareConfigStrip1.isWW = true;
    pinMode(PIN_STRIP_1_WARM_WHITE, OUTPUT);
    HardwareConfigStrip1.PinWarmWhite = PIN_STRIP_1_WARM_WHITE;
#endif

  }

  //---------------- LED Strip 2 ----------------//
  if (CONTROLLER_NUM_LED_STRIPS == 2) {

    //-- Set Hardware Info
    HardwareConfigStrip2.StripID = 2;
    HardwareConfigStrip2.IdValid = true;

    //-- Set Supported Modes
#ifdef STRIP_2_RGB
    HardwareConfigStrip2.isRGB = true;
    pinMode(PIN_STRIP_2_RED,    OUTPUT);
    pinMode(PIN_STRIP_2_GREEN,  OUTPUT);
    pinMode(PIN_STRIP_2_BLUE,   OUTPUT);
    HardwareConfigStrip2.PinRed       = PIN_STRIP_2_RED;
    HardwareConfigStrip2.PinGreen     = PIN_STRIP_2_GREEN;
    HardwareConfigStrip2.PinBlue      = PIN_STRIP_2_BLUE;
#endif

#ifdef STRIP_2_CW
    HardwareConfigStrip2.isCW = true;
    pinMode(PIN_STRIP_2_COLD_WHITE, OUTPUT);
    HardwareConfigStrip2.PinColdWhite = PIN_STRIP_2_COLD_WHITE;
#endif

#ifdef STRIP_2_WW
    HardwareConfigStrip2.isWW = true;
    pinMode(PIN_STRIP_2_WARM_WHITE, OUTPUT);
    HardwareConfigStrip2.PinWarmWhite = PIN_STRIP_2_WARM_WHITE;
#endif

  }

  //---------------- Motion Detection ----------------//
#ifdef CONTROLLER_MOTION_DETECTION
  if (CONTROLLER_NUM_LED_STRIPS <= 1) {
    if (CONTROLLER_NUM_MOTION_SENSORS >= 1) {
      pinMode(PIN_MOTION_SENSOR_1, INPUT);
      attachInterrupt(digitalPinToInterrupt(PIN_MOTION_SENSOR_1), StateMotionDetectedSensor1, CHANGE);
    }
    if (CONTROLLER_NUM_MOTION_SENSORS == 2) {
      pinMode(PIN_MOTION_SENSOR_2, INPUT);
      attachInterrupt(digitalPinToInterrupt(PIN_MOTION_SENSOR_1), StateMotionDetectedSensor2, CHANGE);
    }
  }
#endif

  Serial.println("  Finished Initialization of PINs");

  Serial.println("Finished Setup");
  Serial.println("");

}


void LoopController() {

  //-- Print Loop Time --//
  /*
    unsigned long CurMillisLoop = millis();
    Serial.print(" Loop Time = ");
    Serial.println((CurMillisLoop - PrevMillisLoop));
    PrevMillisLoop = CurMillisLoop;
  */

  mqttClient.loop();

  //-- WiFi --//
  wifi();

  //-- MQTT --//
  mqtt();

  //-- HassIO --//
  hassIO();

  //-- HeartBeat --//
  heartbeat();

  //-- Printer --//
#ifdef INFORMATION_MASTER_PRINT
  printer();
#endif

  //-- LED --//
  leds();


}
