

void printer() {


#ifdef INFORMATION_WIFI_STATE

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
    InformationOneTimePrintWiFiConnected    = false;
    InformationOneTimePrintWiFiDisconnected = true;
  }

  /*-------------------- WiFi Disconnected --------------------*/
  if ((WiFi.status() != WL_CONNECTED) and InformationOneTimePrintWiFiDisconnected) {
    Serial.println("/---------- WiFi Change -----------/");
    Serial.println("   WiFi Disconnected! ");
    Serial.println("/----------------------------------/");
    Serial.println("");
    InformationOneTimePrintWiFiConnected    = true;
    InformationOneTimePrintWiFiDisconnected = false;
  }

#endif


#ifdef INFORMATION_MQTT_STATE

  /*-------------------- MQTT Connected --------------------*/
  if ((mqttClient.connected()) and InformationOneTimePrintMQTTConnected) {
    Serial.println("/---------- MQTT Change -----------/");
    Serial.println("   MQTT Connected! ");
    Serial.println("/----------------------------------/");
    Serial.println("");
    InformationOneTimePrintMQTTConnected    = false;
    InformationOneTimePrintMQTTDisconnected = true;
  }

  /*-------------------- MQTT Disconnected --------------------*/
  if ((!mqttClient.connected()) and InformationOneTimePrintMQTTDisconnected) {
    Serial.println("/---------- MQTT Change -----------/");
    Serial.println("   MQTT Disconnected! ");
    Serial.println("/----------------------------------/");
    Serial.println("");
    InformationOneTimePrintMQTTConnected    = true;
    InformationOneTimePrintMQTTDisconnected = false;
  }

#endif


#ifdef INFORMATION_HASSIO_STATE

  /*-------------------- HassIO Avaiable --------------------*/
  if (!HassIOTimeout and InformationOneTimePrintHassIOAvaibale) {
    Serial.println("/--------- HassIO Change ----------/");
    Serial.println("   HassIO Avaiable! ");
    Serial.println("/----------------------------------/");
    Serial.println("");
    InformationOneTimePrintHassIOAvaibale     = false;
    InformationOneTimePrintHassIONotAvaiable  = true;
  }

  /*-------------------- HassIO Not Avaiable --------------------*/
  if (HassIOTimeout and InformationOneTimePrintHassIONotAvaiable) {
    Serial.println("/--------- HassIO Change ----------/");
    Serial.println("   HassIO Not Avaiable! ");
    Serial.println("/----------------------------------/");
    Serial.println("");
    InformationOneTimePrintHassIOAvaibale     = true;
    InformationOneTimePrintHassIONotAvaiable  = false;
  }

#endif


#ifdef INFORMATION_MQTT_LED

  /*-------------------- First Strip --------------------*/
  if (
    ParameterLEDStrip1.Power      != InfoParameterLEDStrip1.Power       or
    ParameterLEDStrip1.Brightness != InfoParameterLEDStrip1.Brightness  or
#ifdef CONTROLLER_RGBW
    ParameterLEDStrip1.White      != InfoParameterLEDStrip1.White       or
#endif
    ParameterLEDStrip1.Red        != InfoParameterLEDStrip1.Red         or
    ParameterLEDStrip1.Green      != InfoParameterLEDStrip1.Green       or
    ParameterLEDStrip1.Blue       != InfoParameterLEDStrip1.Blue        or
    ParameterLEDStrip1.Effect     != InfoParameterLEDStrip1.Effect
  ) {
    //-- Print Message
    Serial.println("/------ LED Strip 1 Change --------/");
    Serial.print("   Strip 1 Power      : ");
    Serial.println(ParameterLEDStrip1.Power);
    Serial.print("   Strip 1 Brightness : ");
    Serial.println(ParameterLEDStrip1.Brightness);
#ifdef CONTROLLER_RGBW
    Serial.print("   Strip 1 White      : ");
    Serial.println(ParameterLEDStrip1.White);
#endif
    Serial.print("   Strip 1 Red        : ");
    Serial.println(ParameterLEDStrip1.Red);
    Serial.print("   Strip 1 Green      : ");
    Serial.println(ParameterLEDStrip1.Green);
    Serial.print("   Strip 1 Blue       : ");
    Serial.println(ParameterLEDStrip1.Blue);
    Serial.print("   Strip 1 Effect     : ");
    Serial.println(ParameterLEDStrip1.Effect);
    Serial.println("/----------------------------------/");
    Serial.println("");

    //-- Update InfoStruct
    InfoParameterLEDStrip1.Power      = ParameterLEDStrip1.Power;
    InfoParameterLEDStrip1.Brightness = ParameterLEDStrip1.Brightness;
    InfoParameterLEDStrip1.White      = ParameterLEDStrip1.White;
    InfoParameterLEDStrip1.Red        = ParameterLEDStrip1.Red;
    InfoParameterLEDStrip1.Green      = ParameterLEDStrip1.Green;
    InfoParameterLEDStrip1.Blue       = ParameterLEDStrip1.Blue;
    InfoParameterLEDStrip1.Effect     = ParameterLEDStrip1.Effect;
  }

  /*-------------------- Second Strip --------------------*/
  if (
    ParameterLEDStrip2.Power      != InfoParameterLEDStrip2.Power       or
    ParameterLEDStrip2.Brightness != InfoParameterLEDStrip2.Brightness  or
#ifdef CONTROLLER_RGBW
    ParameterLEDStrip2.White      != InfoParameterLEDStrip2.White       or
#endif
    ParameterLEDStrip2.Red        != InfoParameterLEDStrip2.Red         or
    ParameterLEDStrip2.Green      != InfoParameterLEDStrip2.Green       or
    ParameterLEDStrip2.Blue       != InfoParameterLEDStrip2.Blue        or
    ParameterLEDStrip2.Effect     != InfoParameterLEDStrip2.Effect
  ) {
    //-- Print Message
    Serial.println("/------ LED Strip 2 Change --------/");
    Serial.print("   Strip 2 Power      : ");
    Serial.println(ParameterLEDStrip2.Power);
    Serial.print("   Strip 2 Brightness : ");
    Serial.println(ParameterLEDStrip2.Brightness);
#ifdef CONTROLLER_RGBW
    Serial.print("   Strip 2 White      : ");
    Serial.println(ParameterLEDStrip2.White);
#endif
    Serial.print("   Strip 2 Red        : ");
    Serial.println(ParameterLEDStrip2.Red);
    Serial.print("   Strip 2 Green      : ");
    Serial.println(ParameterLEDStrip2.Green);
    Serial.print("   Strip 2 Blue       : ");
    Serial.println(ParameterLEDStrip2.Blue);
    Serial.print("   Strip 2 Effect     : ");
    Serial.println(ParameterLEDStrip2.Effect);
    Serial.println("/----------------------------------/");
    Serial.println("");

    //-- Update InfoStruct
    InfoParameterLEDStrip2.Power      = ParameterLEDStrip2.Power;
    InfoParameterLEDStrip2.Brightness = ParameterLEDStrip2.Brightness;
    InfoParameterLEDStrip2.White      = ParameterLEDStrip2.White;
    InfoParameterLEDStrip2.Red        = ParameterLEDStrip2.Red;
    InfoParameterLEDStrip2.Green      = ParameterLEDStrip2.Green;
    InfoParameterLEDStrip2.Blue       = ParameterLEDStrip2.Blue;
    InfoParameterLEDStrip2.Effect     = ParameterLEDStrip2.Effect;
  }

#endif


#ifdef INFORMATION_MQTT_MOTION_DETECTION

  if (
    ParameterMotion.Power      != InfoParameterMotion.Power   or
    ParameterMotion.Red        != InfoParameterMotion.Red     or
    ParameterMotion.Green      != InfoParameterMotion.Green   or
    ParameterMotion.Blue       != InfoParameterMotion.Blue    or
    ParameterMotion.Timeout    != InfoParameterMotion.Timeout
  ) {
    //-- Print Message
    Serial.println("/----- Motion Detection Change ----/");
    Serial.print("   Motion Detec Power      : ");
    Serial.println(ParameterMotion.Power);
    Serial.print("   Motion Detec Red        : ");
    Serial.println(ParameterMotion.Red);
    Serial.print("   Motion Detec Green      : ");
    Serial.println(ParameterMotion.Green);
    Serial.print("   Motion Detec Blue       : ");
    Serial.println(ParameterMotion.Blue);
    Serial.print("   Motion Detec Timeout    : ");
    Serial.println(ParameterMotion.Timeout);
    Serial.println("/----------------------------------/");
    Serial.println("");

    //-- Update InfoStruct
    InfoParameterMotion.Power   = ParameterMotion.Power;
    InfoParameterMotion.Red     = ParameterMotion.Red;
    InfoParameterMotion.Green   = ParameterMotion.Green;
    InfoParameterMotion.Blue    = ParameterMotion.Blue;
    InfoParameterMotion.Timeout = ParameterMotion.Timeout;
  }

#endif


#ifdef INFORMATION_MQTT_HASSIO

  if (
    ParameterHassIO.SunBelowHorizon   != InfoParameterHassIO.SunBelowHorizon   or
    ParameterHassIO.TimeHour          != InfoParameterHassIO.TimeHour          or
    ParameterHassIO.TimeMin           != InfoParameterHassIO.TimeMin
  ) {
    //-- Print Message
    Serial.println("/---------- HassIO Change ---------/");
    Serial.print("   HassIO Sun         : ");
    Serial.println(ParameterHassIO.SunBelowHorizon);
    Serial.print("   HassIO Time Hour   : ");
    Serial.println(ParameterHassIO.TimeHour);
    Serial.print("   HassIO Time Min    : ");
    Serial.println(ParameterHassIO.TimeMin);
    Serial.println("/----------------------------------/");
    Serial.println("");

    //-- Update InfoStruct
    InfoParameterHassIO.SunBelowHorizon   = ParameterHassIO.SunBelowHorizon;
    InfoParameterHassIO.TimeHour          = ParameterHassIO.TimeHour;
    InfoParameterHassIO.TimeMin           = ParameterHassIO.TimeMin;
  }

#endif


#ifdef INFORMATION_MAIN_STATE

#endif

}
