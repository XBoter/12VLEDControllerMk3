
//------------------------------------- MQTT Callback -------------------------------------//
void callback(char* topic, byte * payload, unsigned int length) {

  //-- Get Message and add terminator
  char message[length + 1];
  for (int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0';

  //######################################## General ########################################//
  if (String(mqtt_command_HassIO_Heartbeat).equals(topic)) {
    //Serial.println(message);
  }

  //######################################## Specific ########################################//
  if (String(mqtt_led_strip_1_json_command).equals(topic)) {
    Serial.println(message);
    mqtt_Client.publish(mqtt_led_strip_1_json_state, message);
  }

  if (String(mqtt_led_strip_2_json_command).equals(topic)) {
    Serial.println(message);
    mqtt_Client.publish(mqtt_led_strip_2_json_state, message);
  }

}
