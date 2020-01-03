

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
    LEDStrip2PinConfig.StripID = 1;
    LEDStrip2PinConfig.IdValid = true;
#ifdef CONTROLLER_RGB or CONTROLLER_RGBW
    //-- Set Pin Modes
    pinMode(PIN_STRIP_1_RED, OUTPUT);
    pinMode(PIN_STRIP_1_GREEN, OUTPUT);
    pinMode(PIN_STRIP_1_BLUE, OUTPUT);
    //-- Set Pin IDs in struct
    LEDStrip1PinConfig.Red    = PIN_STRIP_1_RED;
    LEDStrip1PinConfig.Green  = PIN_STRIP_1_GREEN;
    LEDStrip1PinConfig.Blue   = PIN_STRIP_1_BLUE;
    //-- Set RGB Mode
    LEDStrip1PinConfig.isRGB  = true;
#endif
#ifdef CONTROLLER_RGBW
    //-- Set Pin Modes
    pinMode(PIN_STRIP_1_WHITE, OUTPUT);
    //-- Set Pin IDs in struct
    LEDStrip1PinConfig.White  = PIN_STRIP_1_WHITE;
    //-- Set RGBW Mode
    LEDStrip1PinConfig.isRGBW = true;
#endif
  }

  //---------------- LED Strip 2 ----------------//
  if (CONTROLLER_NUM_LED_STRIPS == 2) {
    LEDStrip2PinConfig.StripID = 2;
    LEDStrip2PinConfig.IdValid = true;
#ifdef CONTROLLER_RGB or CONTROLLER_RGBW
    //-- Set Pin Modes
    pinMode(PIN_STRIP_2_RED, OUTPUT);
    pinMode(PIN_STRIP_2_GREEN, OUTPUT);
    pinMode(PIN_STRIP_2_BLUE, OUTPUT);
    //-- Set Pin IDs in struct
    LEDStrip2PinConfig.Red    = PIN_STRIP_2_RED;
    LEDStrip2PinConfig.Green  = PIN_STRIP_2_GREEN;
    LEDStrip2PinConfig.Blue   = PIN_STRIP_2_BLUE;
    //-- Set RGB Mode
    LEDStrip1PinConfig.isRGB  = true;
#endif
#ifdef CONTROLLER_RGBW
    //-- Set Pin Modes
    pinMode(PIN_STRIP_2_WHITE, OUTPUT);
    //-- Set Pin IDs in struct
    LEDStrip2PinConfig.White  = PIN_STRIP_2_WHITE;
    //-- Set RGBW Mode
    LEDStrip2PinConfig.isRGBW = true;
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
  printer();

  //-- LED --//
  leds();


}
