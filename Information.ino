

void printer() {

#ifdef INFORMATION_SYSTEM

#endif


#ifdef INFORMATION_WIFI

  /*-------------------- WiFi Connected --------------------*/
  if ((WiFi.status() == WL_CONNECTED) and InformationOneTimePrintWiFiConnected) {
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
    InformationOneTimePrintWiFiConnected = false;
    InformationOneTimePrintWiFiDisconnected = true;
  }

  /*-------------------- WiFi Disconnected --------------------*/
  if ((WiFi.status() != WL_CONNECTED) and InformationOneTimePrintWiFiDisconnected) {
    Serial.println("/---------- WiFi Change -----------/");
    Serial.println("   WiFi Disconnected! ");
    Serial.println("/----------------------------------/");
    Serial.println("");
    InformationOneTimePrintWiFiConnected = true;
    InformationOneTimePrintWiFiDisconnected = false;
  }

#endif


#ifdef INFORMATION_MQTT

  /*-------------------- MQTT Connected --------------------*/
  if ((mqttClient.connected()) and InformationOneTimePrintMQTTConnected) {
    Serial.println("/---------- MQTT Change -----------/");
    Serial.println("   MQTT Connected! ");
    Serial.println("/----------------------------------/");
    Serial.println("");
    InformationOneTimePrintMQTTConnected = false;
    InformationOneTimePrintMQTTDisconnected = true;
  }

  /*-------------------- MQTT Disconnected --------------------*/
  if ((!mqttClient.connected()) and InformationOneTimePrintMQTTDisconnected) {
    Serial.println("/---------- MQTT Change -----------/");
    Serial.println("   MQTT Disconnected! ");
    Serial.println("/----------------------------------/");
    Serial.println("");
    InformationOneTimePrintMQTTConnected = true;
    InformationOneTimePrintMQTTDisconnected = false;
  }

#endif


#ifdef INFORMATION_HASSIO

  /*-------------------- HassIO Avaiable --------------------*/
  if (!HassIOTimeout and InformationOneTimePrintHassIOAvaibale) {
    Serial.println("/--------- HassIO Change ----------/");
    Serial.println("   HassIO Avaiable! ");
    Serial.println("/----------------------------------/");
    Serial.println("");
    InformationOneTimePrintHassIOAvaibale = false;
    InformationOneTimePrintHassIONotAvaiable = true;
  }

  /*-------------------- HassIO Not Avaiable --------------------*/
  if (HassIOTimeout and InformationOneTimePrintHassIONotAvaiable) {
    Serial.println("/--------- HassIO Change ----------/");
    Serial.println("   HassIO Not Avaiable! ");
    Serial.println("/----------------------------------/");
    Serial.println("");
    InformationOneTimePrintHassIOAvaibale = true;
    InformationOneTimePrintHassIONotAvaiable = false;
  }

#endif


#ifdef INFORMATION_LED_STRIP

  /*-------------------- First Strip --------------------*/
  if (
    FirstStrip.Power != InfoFirstStrip.Power or
    FirstStrip.Brightness != InfoFirstStrip.Brightness or
    FirstStrip.White != InfoFirstStrip.White or
    FirstStrip.Red != InfoFirstStrip.Red or
    FirstStrip.Green != InfoFirstStrip.Green or
    FirstStrip.Blue != InfoFirstStrip.Blue or
    FirstStrip.Effect != InfoFirstStrip.Effect
  ) {
    //-- Print Message
    Serial.println("/------ LED Strip 1 Change --------/");
    Serial.print("   Strip 1 Power      : ");
    Serial.println(FirstStrip.Power);
    Serial.print("   Strip 1 Brightness : ");
    Serial.println(FirstStrip.Brightness);
    Serial.print("   Strip 1 White      : ");
    Serial.println(FirstStrip.White);
    Serial.print("   Strip 1 Red        : ");
    Serial.println(FirstStrip.Red);
    Serial.print("   Strip 1 Green      : ");
    Serial.println(FirstStrip.Green);
    Serial.print("   Strip 1 Blue       : ");
    Serial.println(FirstStrip.Blue);
    Serial.print("   Strip 1 Effect     : ");
    Serial.println(FirstStrip.Effect);
    Serial.println("/----------------------------------/");
    Serial.println("");

    //-- Update InfoStruct
    InfoFirstStrip.Power = FirstStrip.Power;
    InfoFirstStrip.Brightness = FirstStrip.Brightness;
    InfoFirstStrip.White = FirstStrip.White;
    InfoFirstStrip.Red = FirstStrip.Red;
    InfoFirstStrip.Green = FirstStrip.Green;
    InfoFirstStrip.Blue = FirstStrip.Blue;
    InfoFirstStrip.Effect = FirstStrip.Effect;
  }

  /*-------------------- Second Strip --------------------*/
  if (
    SecondStrip.Power != InfoSecondStrip.Power or
    SecondStrip.Brightness != InfoSecondStrip.Brightness or
    SecondStrip.White != InfoSecondStrip.White or
    SecondStrip.Red != InfoSecondStrip.Red or
    SecondStrip.Green != InfoSecondStrip.Green or
    SecondStrip.Blue != InfoSecondStrip.Blue or
    SecondStrip.Effect != InfoSecondStrip.Effect
  ) {
    //-- Print Message
    Serial.println("/------ LED Strip 2 Change --------/");
    Serial.print("   Strip 2 Power      : ");
    Serial.println(SecondStrip.Power);
    Serial.print("   Strip 2 Brightness : ");
    Serial.println(SecondStrip.Brightness);
    Serial.print("   Strip 2 White      : ");
    Serial.println(SecondStrip.White);
    Serial.print("   Strip 2 Red        : ");
    Serial.println(SecondStrip.Red);
    Serial.print("   Strip 2 Green      : ");
    Serial.println(SecondStrip.Green);
    Serial.print("   Strip 2 Blue       : ");
    Serial.println(SecondStrip.Blue);
    Serial.print("   Strip 2 Effect     : ");
    Serial.println(SecondStrip.Effect);
    Serial.println("/----------------------------------/");
    Serial.println("");

    //-- Update InfoStruct
    InfoSecondStrip.Power = SecondStrip.Power;
    InfoSecondStrip.Brightness = SecondStrip.Brightness;
    InfoSecondStrip.White = SecondStrip.White;
    InfoSecondStrip.Red = SecondStrip.Red;
    InfoSecondStrip.Green = SecondStrip.Green;
    InfoSecondStrip.Blue = SecondStrip.Blue;
    InfoSecondStrip.Effect = SecondStrip.Effect;
  }

#endif


#ifdef INFORMATION_MAIN_STATE

#endif

}
