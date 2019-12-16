

void printer() {

#ifdef INFORMATION_SYSTEM

  /*-------------------- System Reboot --------------------*/
  if (Information_mqtt_System_Reboot != mqtt_System_Reboot) {
    Serial.println("/--------- System Change ----------/");
    Serial.print("   System Reboot     : ");
    Serial.println(mqtt_System_Reboot);
    Serial.println("/----------------------------------/");
    Serial.println("");
    Information_mqtt_System_Reboot = mqtt_System_Reboot;
  }

#endif


#ifdef INFORMATION_WIFI

  /*-------------------- WiFi Connected --------------------*/
  if ((WiFi.status() == WL_CONNECTED) and OneTimePrintWiFiConnected) {
    Serial.println("/---------- WiFi Change -----------/");
    Serial.println("   WiFi Connected! ");
    Serial.println("/----------------------------------/");
    Serial.println("");
    Serial.println("/----------- WiFi Info ------------/");
    Serial.print("   IP Address        : ");
    Serial.println(WiFi.localIP());
    Serial.print("   Subnetmask        : ");
    Serial.println(WiFi.subnetMask());
    Serial.print("   MAC Address       : ");
    Serial.println(WiFi.macAddress());
    Serial.print("   Gateway           : ");
    Serial.println(WiFi.gatewayIP());
    Serial.println("/----------------------------------/");
    Serial.println("");
    OneTimePrintWiFiConnected = false;
    OneTimePrintWiFiDisconnected = true;
  }

  /*-------------------- WiFi Disconnected --------------------*/
  if ((WiFi.status() != WL_CONNECTED) and OneTimePrintWiFiDisconnected) {
    Serial.println("/---------- WiFi Change -----------/");
    Serial.println("   WiFi Disconnected! ");
    Serial.println("/----------------------------------/");
    Serial.println("");
    OneTimePrintWiFiDisconnected = false;
    OneTimePrintWiFiConnected = true;
  }

#endif


#ifdef INFORMATION_MQTT

  //------------------- Parameter [mqtt_System_Reboot] -------------------//
  if (mqtt_System_Reboot != Information_mqtt_System_Reboot) {
    Serial.println("/----- MQTT Parameter Change ------/");
    Serial.print("   System Reboot        : ");
    Serial.println(mqtt_System_Reboot);
    Serial.println("/----------------------------------/");
    Serial.println("");
    Information_mqtt_System_Reboot = mqtt_System_Reboot;
  }

#endif


#ifdef INFORMATION_MAIN_STATE

  /*-------------------- Main State --------------------*/
  if (Information_MainState != MainState) {
    Serial.println("/------- Main State Change --------/");
    Serial.print("   Main State        : ");
    Serial.println(MainState);
    Serial.println("/----------------------------------/");
    Serial.println("");
    Information_MainState = MainState;
  }

#endif

}
