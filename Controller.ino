

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

  Serial.println("  Start PINs Initialization");

  //---------------- LED Strip 1 ----------------//
  pinMode(PIN_STRIP_1_RED, OUTPUT);
  pinMode(PIN_STRIP_1_GREEN, OUTPUT);
  pinMode(PIN_STRIP_1_BLUE, OUTPUT);
  pinMode(PIN_STRIP_1_WHITE, OUTPUT);

  //---------------- LED Strip 2 ----------------//
  pinMode(PIN_STRIP_2_RED, OUTPUT);
  pinMode(PIN_STRIP_2_GREEN, OUTPUT);
  pinMode(PIN_STRIP_2_BLUE, OUTPUT);
  pinMode(PIN_STRIP_2_WHITE, OUTPUT);

  Serial.println("  Finished Initialization of PINs");

  //---- Init Variables
  PrevMillisNoHassIOConnection = millis();

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
