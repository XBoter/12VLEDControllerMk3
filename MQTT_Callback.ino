
//------------------------------------- MQTT Callback -------------------------------------//
void callback(char* topic, byte * payload, int length) {

  //-- Get Message and add terminator
  char message[length + 1];
  for (int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0';

  //######################################## General ########################################//
  if (String(mqtt_HassIO_Heartbeat_command).equals(topic)) {
    PrevMillisNoHassIOConnection = millis();
  }

  //######################################## Specific ########################################//
  //---- LED Strip 1 Power
  if (String(mqtt_strip1_power_command_topic).equals(topic)) {
    mqttClient.publish(mqtt_strip1_power_state_topic, message);
    FirstStrip.Power = atoi(message);
  }

  //---- LED Strip 1 Brightness
  if (String(mqtt_strip1_brightness_command_topic).equals(topic)) {
    mqttClient.publish(mqtt_strip1_brightness_state_topic, message);
    FirstStrip.Brightness = atoi(message);
  }

  //---- LED Strip 1 White
  if (String(mqtt_strip1_white_value_command_topic).equals(topic)) {
    mqttClient.publish(mqtt_strip1_white_value_state_topic, message);
    FirstStrip.White = atoi(message);
  }

  //---- LED Strip 1 RGB
  if (String(mqtt_strip1_rgb_command_topic).equals(topic)) {
    mqttClient.publish(mqtt_strip1_rgb_state_topic, message);
    strcpy(LastColorStrip1Holder, message);
    FirstStrip.Red    = atoi(strtok(message, ","));
    FirstStrip.Green  = atoi(strtok(NULL, ","));
    FirstStrip.Blue   = atoi(strtok(NULL, ","));
  }

  //---- LED Strip 1 Effect
  if (String(mqtt_strip1_effect_command_topic).equals(topic)) {
    mqttClient.publish(mqtt_strip1_effect_state_topic, message);
    strcpy(LastEffectStrip2Holder, message);
    String effect = strtok(message, "\0");
    if (effect.equals("None")) {
      FirstStrip.Effect = None;
    } else if (effect.equals("Alarm")) {
      FirstStrip.Effect = Alarm;
    } else if (effect.equals("Wakeup")) {
      FirstStrip.Effect = Wakeup;
    } else if (effect.equals("Sleep")) {
      FirstStrip.Effect = Sleep;
    } else if (effect.equals("Weekend")) {
      FirstStrip.Effect = Weekend;
    }
  }


  //---- LED Strip 2 Power
  if (String(mqtt_strip2_power_command_topic).equals(topic)) {
    mqttClient.publish(mqtt_strip2_power_state_topic, message);
    SecondStrip.Power = atoi(message);
  }

  //---- LED Strip 2 Brightness
  if (String(mqtt_strip2_brightness_command_topic).equals(topic)) {
    mqttClient.publish(mqtt_strip2_brightness_state_topic, message);
    SecondStrip.Brightness = atoi(message);
  }

  //---- LED Strip 2 White
  if (String(mqtt_strip2_white_value_command_topic).equals(topic)) {
    mqttClient.publish(mqtt_strip2_white_value_state_topic, message);
    SecondStrip.White = atoi(message);
  }

  //---- LED Strip 2 RGB
  if (String(mqtt_strip2_rgb_command_topic).equals(topic)) {
    mqttClient.publish(mqtt_strip2_rgb_state_topic, message);
    strcpy(LastColorStrip2Holder, message);
    SecondStrip.Red    = atoi(strtok(message, ","));
    SecondStrip.Green  = atoi(strtok(NULL, ","));
    SecondStrip.Blue   = atoi(strtok(NULL, ","));
  }

  //---- LED Strip 2 Effect
  if (String(mqtt_strip2_effect_command_topic).equals(topic)) {
    mqttClient.publish(mqtt_strip2_effect_state_topic, message);
    strcpy(LastEffectStrip2Holder, message);
    String effect = strtok(message, "\0");
    if (effect.equals("None")) {
      SecondStrip.Effect = None;
    } else if (effect.equals("Alarm")) {
      SecondStrip.Effect = Alarm;
    } else if (effect.equals("Wakeup")) {
      SecondStrip.Effect = Wakeup;
    } else if (effect.equals("Sleep")) {
      SecondStrip.Effect = Sleep;
    } else if (effect.equals("Weekend")) {
      SecondStrip.Effect = Weekend;
    }
  }

}
