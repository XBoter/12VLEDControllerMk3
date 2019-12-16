

void printer() {

#ifdef INFORMATION_SYSTEM

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
  }

#endif


#ifdef INFORMATION_MQTT

#endif


#ifdef INFORMATION_MAIN_STATE

#endif

}
