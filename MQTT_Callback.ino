
//------------------------------------- MQTT Callback -------------------------------------//
void callback(char* topic, byte * payload, int length) {

  //-- Get Message and add terminator
  char message[length + 1];
  for (int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0';

  //######################################## Global ########################################//
  //---- HassIO Heartbeat
  if (String(mqtt_HassIO_Heartbeat_command).equals(topic)) {
    PrevMillisNoHassIOConnection = millis();
  }

  //---- HassIO Time
  if (String(mqtt_HassIO_time_command).equals(topic)) {
    String hour   = strtok(message, ":");
    String minute = strtok(NULL, "\0");
    ParameterHassIO.TimeHour = hour.toInt();
    ParameterHassIO.TimeMin  = minute.toInt();
  }

  //---- HassIO Sun
  if (String(mqtt_HassIO_sun_command).equals(topic)) {
    if (!CheckIfBool(atoi(message))) {
      return;
    }
    ParameterHassIO.SunBelowHorizon = atoi(message);
  }

  //---- Global Good Night
  if (String(mqtt_global_good_night_command).equals(topic)) {

  }

  //---- Global Good Morning
  if (String(mqtt_global_good_morning_command).equals(topic)) {

  }

  //######################################## General ########################################//
  //---- Motion Detection Power
  if (String(mqtt_motion_detection_power_command).equals(topic)) {
    if (!CheckIfBool(atoi(message))) {
      return;
    }
    ParameterMotion.Power = atoi(message);
  }

  //---- Motion Detection RGB
  if (String(mqtt_motion_detection_rgb_command).equals(topic)) {
    //-- Check Red Value
    int red = atoi(strtok(message, ","));
    if (!CheckIfUint8(red)) {
      return;
    }
    ParameterMotion.Red = red;

    //-- Check Green Value
    int green = atoi(strtok(NULL, ","));
    if (!CheckIfUint8(green)) {
      return;
    }
    ParameterMotion.Green = green;

    //-- Check Blue Value
    int blue = atoi(strtok(NULL, ","));
    if (!CheckIfUint8(blue)) {
      return;
    }
    ParameterMotion.Blue = blue;
  }

  //---- Motion Detection Timeout
  if (String(mqtt_motion_detection_timeout_command).equals(topic)) {
    ParameterMotion.Timeout = atoi(message);
  }


  //######################################## Specific ########################################//
  //---- LED Strip 1 Power
  if (String(mqtt_strip1_power_command).equals(topic)) {
    if (!CheckIfBool(atoi(message))) {
      return;
    }
    mqttClient.publish(mqtt_strip1_power_state, message);
    ParameterLEDStrip1.Power = atoi(message);
  }

  //---- LED Strip 1 Brightness
  if (String(mqtt_strip1_brightness_command).equals(topic)) {

    //-- Check if Brightness is needed
    if (HardwareConfigStrip1.isRGB or HardwareConfigStrip1.isCW or HardwareConfigStrip1.isWW) {

      mqttClient.publish(mqtt_strip1_brightness_state, message);

      //-- Check Brightness Value
      int brightness = atoi(message);
      if (!CheckIfUint8(brightness)) {
        return;
      }
      ParameterLEDStrip1.Brightness = brightness;

    }

  }

  //---- LED Strip 1 RGB
  if (String(mqtt_strip1_rgb_command).equals(topic)) {

    //-- Check if RGB is supported
    if (HardwareConfigStrip1.isRGB) {

      mqttClient.publish(mqtt_strip1_rgb_state, message);
      strcpy(LastColorStrip1Holder, message);

      //-- Check Red Value
      int red = atoi(strtok(message, ","));
      if (!CheckIfUint8(red)) {
        return;
      }
      ParameterLEDStrip1.Red = red;

      //-- Check Green Value
      int green = atoi(strtok(NULL, ","));
      if (!CheckIfUint8(green)) {
        return;
      }
      ParameterLEDStrip1.Green = green;

      //-- Check Blue Value
      int blue = atoi(strtok(NULL, ","));
      if (!CheckIfUint8(blue)) {
        return;
      }
      ParameterLEDStrip1.Blue = blue;

    }
  }

  //---- LED Strip 1 Cold White
  if (String(mqtt_strip1_cold_white_value_command).equals(topic)) {

    //-- Check if CW is supported
    if (HardwareConfigStrip1.isCW) {

      mqttClient.publish(mqtt_strip1_cold_white_value_state, message);

      //-- Check Cold White Value
      int ColdWhite = atoi(message);
      if (!CheckIfUint8(ColdWhite)) {
        return;
      }
      ParameterLEDStrip1.ColdWhite = ColdWhite;

    }

  }

  //---- LED Strip 1 Warm White
  if (String(mqtt_strip1_warm_white_value_command).equals(topic)) {

    //-- Check if WW is supported
    if (HardwareConfigStrip1.isWW) {

      mqttClient.publish(mqtt_strip1_warm_white_value_state, message);

      //-- Check Warm White Value
      int WarmWhite = atoi(message);
      if (!CheckIfUint8(WarmWhite)) {
        return;
      }
      ParameterLEDStrip1.WarmWhite = WarmWhite;

    }

  }

  //---- LED Strip 1 Effect
  if (String(mqtt_strip1_effect_command).equals(topic)) {
    mqttClient.publish(mqtt_strip1_effect_state, message);
    strcpy(LastEffectStrip2Holder, message);
    String effect = strtok(message, "\0");
    if (effect.equals("None")) {
      ParameterLEDStrip1.Effect = e_None;
    } else if (effect.equals("Alarm")) {
      ParameterLEDStrip1.Effect = e_Alarm;
    } else if (effect.equals("Wakeup")) {
      ParameterLEDStrip1.Effect = e_Wakeup;
    } else if (effect.equals("Sleep")) {
      ParameterLEDStrip1.Effect = e_Sleep;
    } else if (effect.equals("Weekend")) {
      ParameterLEDStrip1.Effect = e_Weekend;
    }
  }


  //---- LED Strip 2 Power
  if (String(mqtt_strip2_power_command).equals(topic)) {
    if (!CheckIfBool(atoi(message))) {
      return;
    }
    mqttClient.publish(mqtt_strip2_power_state, message);
    ParameterLEDStrip2.Power = atoi(message);
  }

  //---- LED Strip 2 Brightness
  if (String(mqtt_strip2_brightness_command).equals(topic)) {

    //-- Check if Brightness is needed
    if (HardwareConfigStrip2.isRGB or HardwareConfigStrip2.isCW or HardwareConfigStrip2.isWW) {

      mqttClient.publish(mqtt_strip2_brightness_state, message);

      //-- Check Brightness Value
      int brightness = atoi(message);
      if (!CheckIfUint8(brightness)) {
        return;
      }
      ParameterLEDStrip2.Brightness = brightness;

    }

  }

  //---- LED Strip 2 RGB
  if (String(mqtt_strip2_rgb_command).equals(topic)) {

    //-- Check if RGB is supported
    if (HardwareConfigStrip2.isRGB) {

      mqttClient.publish(mqtt_strip2_rgb_state, message);
      strcpy(LastColorStrip2Holder, message);

      //-- Check Red Value
      int red = atoi(strtok(message, ","));
      if (!CheckIfUint8(red)) {
        return;
      }
      ParameterLEDStrip2.Red = red;

      //-- Check Green Value
      int green = atoi(strtok(NULL, ","));
      if (!CheckIfUint8(green)) {
        return;
      }
      ParameterLEDStrip2.Green = green;

      //-- Check Blue Value
      int blue = atoi(strtok(NULL, ","));
      if (!CheckIfUint8(blue)) {
        return;
      }
      ParameterLEDStrip2.Blue = blue;

    }

  }

  //---- LED Strip 2 Cold White
  if (String(mqtt_strip2_cold_white_value_command).equals(topic)) {

    //-- Check if CW is supported
    if (HardwareConfigStrip2.isCW) {

      mqttClient.publish(mqtt_strip2_cold_white_value_state, message);

      //-- Check Cold White Value
      int ColdWhite = atoi(message);
      if (!CheckIfUint8(ColdWhite)) {
        return;
      }
      ParameterLEDStrip2.ColdWhite = ColdWhite;

    }

  }

  //---- LED Strip 2 Warm White
  if (String(mqtt_strip2_warm_white_value_command).equals(topic)) {

    //-- Check if CW is supported
    if (HardwareConfigStrip2.isWW) {

      mqttClient.publish(mqtt_strip2_warm_white_value_state, message);

      //-- Check Warm White Value
      int WarmWhite = atoi(message);
      if (!CheckIfUint8(WarmWhite)) {
        return;
      }
      ParameterLEDStrip2.WarmWhite = WarmWhite;

    }

  }

  //---- LED Strip 2 Effect
  if (String(mqtt_strip2_effect_command).equals(topic)) {
    mqttClient.publish(mqtt_strip2_effect_state, message);
    strcpy(LastEffectStrip2Holder, message);
    String effect = strtok(message, "\0");
    if (effect.equals("None")) {
      ParameterLEDStrip2.Effect = e_None;
    } else if (effect.equals("Alarm")) {
      ParameterLEDStrip2.Effect = e_Alarm;
    } else if (effect.equals("Wakeup")) {
      ParameterLEDStrip2.Effect = e_Wakeup;
    } else if (effect.equals("Sleep")) {
      ParameterLEDStrip2.Effect = e_Sleep;
    } else if (effect.equals("Weekend")) {
      ParameterLEDStrip2.Effect = e_Weekend;
    }
  }

}


boolean CheckIfBool(int x) {
  boolean y = false;
  if (x == 0 or x == 1) {
    y = true;
  }
  return y;
}


boolean CheckIfUint8(int x) {
  boolean y = false;
  if (x >= 0 and x <= 255) {
    y = true;
  }
  return y;
}
