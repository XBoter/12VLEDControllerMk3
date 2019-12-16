

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

  Serial.println("  Start Initialization of Dummy");
  //Put Setup code here!
  Serial.println("  Finished Initialization of Dummy");

  Serial.println("Finished Setup");
  Serial.println("");
}


void LoopController() {

  mqtt_Client.loop();


  //-- WiFi --//
  if (WiFi.status() != WL_CONNECTED) {
    wifi();
  }


  //-- MQTT --//
  if ((WiFi.status() == WL_CONNECTED) and (!mqtt_Client.connected())) {
    mqtt();
  }


  //-- HeartBeat --//
  if ((WiFi.status() == WL_CONNECTED) and (mqtt_Client.connected())) {
    heartbeat();
  }


  //-- Printer --//
  printer();


  //-- Controller --//
  switch (MainState) {

    case 0:
      break;

    case 10:
      break;

    case 20:
      break;

    case 99:
      break;

    default:
      break;

  }

  //-- System Reboot --//
  if (mqtt_System_Reboot) {
    unsigned long CurMillis_SystemRebootDelay = millis();
    if (CurMillis_SystemRebootDelay - PrevMillis_SystemRebootDelay >= TimeOut_SystemRebootDelay) {
      PrevMillis_SystemRebootDelay = CurMillis_SystemRebootDelay;
      Serial.println("#!# REBOOTING #!#");
      ESP.restart();
    }
  }
}
