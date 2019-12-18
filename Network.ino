

void wifi() {

  // Get current millis for WiFi reconnect Timeout
  unsigned long CurMillisNoWiFiConnection = millis();

  switch (WiFiState) {

    //-- Try Connecting to WiFi
    case 0:
      WiFi.mode(WIFI_STA);
      WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
      PrevMillisNoWiFiConnection = CurMillisNoWiFiConnection; // Set prevtime for connection timeout
      WiFiState = 1; // Wait for WiFi to connect
      break;


    //-- Check if WiFi is Connected
    case 1:
      // Check if WiFI is connected
      if (WiFi.status() == WL_CONNECTED) {
        WiFiState = 10; // Check if WiFi disconnects
      }
      // Wait 5 seconds for the WiFI to connect after that try again
      if (CurMillisNoWiFiConnection - PrevMillisNoWiFiConnection >= TimeOutNoWiFiConnection) {
        WiFiState = 2; // Disconnect WiFi
      }
      break;


    //-- Disconnect WiFI
    case 2:
      WiFi.disconnect();
      WiFiState = 0; // Restart WiFi connection
      break;


    //-- Check if WiFI is Disctonnected
    case 10:
      if (WiFi.status() != WL_CONNECTED) {
        WiFiState = 0; // When disconnected reconnect
      }
      break;


    //-- If something crashes start again
    default:
      WiFiState = 0;
      break;
  }

}


void mqtt() {

  // Get current millis for MQTT reconnect Timeout
  unsigned long CurMillisNoMQTTConnection = millis();

  switch (MQTTState) {

    //-- Check if WiFi is connected and the client isn't connected to the MQTT broker
    case 0:
      if ((WiFi.status() == WL_CONNECTED) and !mqttClient.connected()) {
        MQTTState = 1; // Try connecting to the MQTT broker
      }
      break;


    //-- Try connecting to the MQTT broker as long as the WiFi is avaiable
    case 1:
      if (WiFi.status() == WL_CONNECTED) {
        mqttClient.connect(MQTT_CLIENT_NAME, MQTT_USERNAME, MQTT_PASSWORD);
        PrevMillisNoMQTTConnection = CurMillisNoMQTTConnection; // Set prevtime for connection timeout
        MQTTState = 2;
      } else {
        MQTTState = 0; // Check if WiFi is avaiable and client is disconnected
      }
      break;


    //-- If connected to the WiFi and the broker subscribe to the channel. If not after 5 seconds try again
    case 2:
      if ((WiFi.status() == WL_CONNECTED) and mqttClient.connected()) {

        //*** General ***//
        mqttClient.subscribe(mqtt_HassIO_Heartbeat_command);    // HassIO sends a Heartbeat every second

        //*** Specific ***//
        //-- LED Strip 1
        mqttClient.subscribe(mqtt_strip1_power_command_topic);
        mqttClient.subscribe(mqtt_strip1_brightness_command_topic);
        mqttClient.subscribe(mqtt_strip1_white_value_command_topic);
        mqttClient.subscribe(mqtt_strip1_rgb_command_topic);
        mqttClient.subscribe(mqtt_strip1_effect_command_topic);

        //-- LED Strip 2
        mqttClient.subscribe(mqtt_strip2_power_command_topic);
        mqttClient.subscribe(mqtt_strip2_brightness_command_topic);
        mqttClient.subscribe(mqtt_strip2_white_value_command_topic);
        mqttClient.subscribe(mqtt_strip2_rgb_command_topic);
        mqttClient.subscribe(mqtt_strip2_effect_command_topic);

        MQTTState = 0;
      } else {
        // Wait 5 seconds to connect to the MQTT broker after that try again
        if (CurMillisNoMQTTConnection - PrevMillisNoMQTTConnection >= TimeOutNoMQTTConnection) {
          MQTTState = 0; // Check if WiFi is aviable and client is disconnected
        }
      }
      break;


    //-- If something crashes start again
    default:
      MQTTState = 0;
      break;

  }

}

void hassIO() {
  //-- Check if HassIO is down
  unsigned long CurMillisNoHassIOConnection = millis();
  if (CurMillisNoHassIOConnection - PrevMillisNoHassIOConnection >= TimeOutNoHassIOConnection) {
    HassIOTimeout = true;
    SendMqttParameter = true;
  } else {
    HassIOTimeout = false;
  }

  //-- If HassIO is avaiable again send data
  if (SendMqttParameter and !HassIOTimeout) {
    SendMqttParameter = false;
    char tempValueHolder[32];

    //-- LED Strip 1
    sprintf(tempValueHolder, "%d",  FirstStrip.Power);
    mqttClient.publish(mqtt_strip1_power_state_topic, tempValueHolder);
    sprintf(tempValueHolder, "%d",  FirstStrip.Brightness);
    mqttClient.publish(mqtt_strip1_brightness_state_topic, tempValueHolder);
    sprintf(tempValueHolder, "%d",  FirstStrip.White);
    mqttClient.publish(mqtt_strip1_white_value_state_topic, tempValueHolder);

    mqttClient.publish(mqtt_strip1_rgb_state_topic, LastColorStrip1Holder);
    mqttClient.publish(mqtt_strip1_effect_state_topic, LastEffectStrip1Holder);

    //-- LED Strip 2
    sprintf(tempValueHolder, "%d",  SecondStrip.Power);
    mqttClient.publish(mqtt_strip2_power_state_topic, tempValueHolder);
    sprintf(tempValueHolder, "%d",  SecondStrip.Brightness);
    mqttClient.publish(mqtt_strip2_brightness_state_topic, tempValueHolder);
    sprintf(tempValueHolder, "%d",  SecondStrip.White);
    mqttClient.publish(mqtt_strip2_white_value_state_topic, tempValueHolder);
    
    mqttClient.publish(mqtt_strip2_rgb_state_topic, LastColorStrip2Holder);
    mqttClient.publish(mqtt_strip2_effect_state_topic, LastEffectStrip2Holder);
  }

}

void heartbeat() {

  //-- Send ESP Heartbeat over MQTT
  unsigned long CurMillisESPHeartBeat = millis();
  if (CurMillisESPHeartBeat - PrevMillisESPHeartBeat >= TimeOutESPHeartBeat) {
    PrevMillisESPHeartBeat = CurMillisESPHeartBeat;
    char message[20];
    sprintf(message, "%ld", ESPHeartBeatCounter);
    mqttClient.publish(mqtt_ESP_Heartbeat_state, message);
    ESPHeartBeatCounter++;
  }

}
