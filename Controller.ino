

void SetupController() {
  Serial.begin(SETUP_BAUD_RATE);
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
  mqtt_Client.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt_Client.setCallback(callback);
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

  Serial.println("Finished Setup");
  Serial.println("");
  
}


void LoopController() {

  mqtt_Client.loop();

  //-- WiFi --//
  wifi();

  //-- MQTT --//
  mqtt();

  //-- HeartBeat --//
  heartbeat();

  //-- Printer --//
  printer();

  //-- LED --//
  leds();
 
}
