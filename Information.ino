

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
#ifdef STRIP_1_RGB
    ParameterLEDStrip1.Red        != InfoParameterLEDStrip1.Red         or
    ParameterLEDStrip1.Green      != InfoParameterLEDStrip1.Green       or
    ParameterLEDStrip1.Blue       != InfoParameterLEDStrip1.Blue        or
#endif
#ifdef STRIP_1_CW
    ParameterLEDStrip1.ColdWhite  != InfoParameterLEDStrip1.ColdWhite   or
#endif
#ifdef STRIP_1_WW
    ParameterLEDStrip1.WarmWhite  != InfoParameterLEDStrip1.WarmWhite   or
#endif
    ParameterLEDStrip1.Effect     != InfoParameterLEDStrip1.Effect
  ) {
    //-- Print Message
    Serial.println("/------ LED Strip 1 Change --------/");
    Serial.print("   Strip 1 Power      : ");
    Serial.println(ParameterLEDStrip1.Power);
    Serial.print("   Strip 1 Brightness : ");
    Serial.println(ParameterLEDStrip1.Brightness);
#ifdef STRIP_1_RGB
    Serial.print("   Strip 1 Red        : ");
    Serial.println(ParameterLEDStrip1.Red);
    Serial.print("   Strip 1 Green      : ");
    Serial.println(ParameterLEDStrip1.Green);
    Serial.print("   Strip 1 Blue       : ");
    Serial.println(ParameterLEDStrip1.Blue);
#endif
#ifdef STRIP_1_CW
    Serial.print("   Strip 1 Cold White : ");
    Serial.println(ParameterLEDStrip1.ColdWhite);
#endif
#ifdef STRIP_1_WW
    Serial.print("   Strip 1 Warm White : ");
    Serial.println(ParameterLEDStrip1.WarmWhite);
#endif
    Serial.print("   Strip 1 Effect     : ");
    Serial.println(ParameterLEDStrip1.Effect);
    Serial.println("/----------------------------------/");
    Serial.println("");

    //-- Update InfoStruct
    InfoParameterLEDStrip1.Power      = ParameterLEDStrip1.Power;
    InfoParameterLEDStrip1.Brightness = ParameterLEDStrip1.Brightness;
    InfoParameterLEDStrip1.Red        = ParameterLEDStrip1.Red;
    InfoParameterLEDStrip1.Green      = ParameterLEDStrip1.Green;
    InfoParameterLEDStrip1.Blue       = ParameterLEDStrip1.Blue;
    InfoParameterLEDStrip1.ColdWhite  = ParameterLEDStrip1.ColdWhite;
    InfoParameterLEDStrip1.WarmWhite  = ParameterLEDStrip1.WarmWhite;
    InfoParameterLEDStrip1.Effect     = ParameterLEDStrip1.Effect;
  }

  /*-------------------- Second Strip --------------------*/
  if (
    ParameterLEDStrip2.Power      != InfoParameterLEDStrip2.Power       or
    ParameterLEDStrip2.Brightness != InfoParameterLEDStrip2.Brightness  or
#ifdef STRIP_2_RGB
    ParameterLEDStrip2.Red        != InfoParameterLEDStrip2.Red         or
    ParameterLEDStrip2.Green      != InfoParameterLEDStrip2.Green       or
    ParameterLEDStrip2.Blue       != InfoParameterLEDStrip2.Blue        or
#endif
#ifdef STRIP_2_CW
    ParameterLEDStrip2.ColdWhite  != InfoParameterLEDStrip2.ColdWhite   or
#endif
#ifdef STRIP_2_WW
    ParameterLEDStrip2.WarmWhite  != InfoParameterLEDStrip2.WarmWhite   or
#endif
    ParameterLEDStrip2.Effect     != InfoParameterLEDStrip2.Effect
  ) {
    //-- Print Message
    Serial.println("/------ LED Strip 2 Change --------/");
    Serial.print("   Strip 2 Power      : ");
    Serial.println(ParameterLEDStrip2.Power);
    Serial.print("   Strip 2 Brightness : ");
    Serial.println(ParameterLEDStrip2.Brightness);
#ifdef STRIP_2_RGB
    Serial.print("   Strip 2 Red        : ");
    Serial.println(ParameterLEDStrip2.Red);
    Serial.print("   Strip 2 Green      : ");
    Serial.println(ParameterLEDStrip2.Green);
    Serial.print("   Strip 2 Blue       : ");
    Serial.println(ParameterLEDStrip2.Blue);
#endif
#ifdef STRIP_2_CW
    Serial.print("   Strip 2 Cold White : ");
    Serial.println(ParameterLEDStrip2.ColdWhite);
#endif
#ifdef STRIP_2_WW
    Serial.print("   Strip 2 Warm White : ");
    Serial.println(ParameterLEDStrip2.WarmWhite);
#endif
    Serial.print("   Strip 2 Effect     : ");
    Serial.println(ParameterLEDStrip2.Effect);
    Serial.println("/----------------------------------/");
    Serial.println("");

    //-- Update InfoStruct
    InfoParameterLEDStrip2.Power      = ParameterLEDStrip2.Power;
    InfoParameterLEDStrip2.Brightness = ParameterLEDStrip2.Brightness;
    InfoParameterLEDStrip2.Red        = ParameterLEDStrip2.Red;
    InfoParameterLEDStrip2.Green      = ParameterLEDStrip2.Green;
    InfoParameterLEDStrip2.Blue       = ParameterLEDStrip2.Blue;
    InfoParameterLEDStrip2.ColdWhite  = ParameterLEDStrip2.ColdWhite;
    InfoParameterLEDStrip2.WarmWhite  = ParameterLEDStrip2.WarmWhite;
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


#ifdef INFORMATION_LIGHT_STATE

  if (TransitionDataStrip1.CurMode != InfoTransitionDataStrip1.CurMode) {

    Serial.println("/------ Mode Strip 1 Change -------/");
    Serial.print("   Current Mode       : ");
    switch (TransitionDataStrip1.CurMode) {
      case Idle:
        Serial.print("Idle");
        break;
      case Normal:
        Serial.print("Normal");
        break;
      case Motion:
        Serial.print("Motion");
        break;
      case NoWiFi:
        Serial.print("NoWiFi");
        break;
      case NoMQTT:
        Serial.print("NoMQTT");
        break;
      case NoHassIO:
        Serial.print("NoHassIO");
        break;
      case Wakeup:
        Serial.print("Wakeup");
        break;
      case Sleep:
        Serial.print("Sleep");
        break;
      case Weekend:
        Serial.print("Weekend");
        break;
      case Alarm:
        Serial.print("Alarm");
        break;
    }
    Serial.println("");
    Serial.println("/----------------------------------/");
    Serial.println("");

    //-- Update InfoStruct
    InfoTransitionDataStrip1.CurMode = TransitionDataStrip1.CurMode;
  }

#endif

}
