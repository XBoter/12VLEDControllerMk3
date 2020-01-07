

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
      NetworkState.Wifi_Connected = true;
      if (WiFi.status() != WL_CONNECTED) {
        NetworkState.Wifi_Connected = false;
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
      NetworkState.MQTT_Connected = true;
      if ((WiFi.status() == WL_CONNECTED) and !mqttClient.connected()) {
        NetworkState.MQTT_Connected = false;
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

        //*** Global ***//
        //-- HassIO
        mqttClient.subscribe(mqtt_HassIO_Heartbeat_command);        // HassIO sends a Heartbeat every second
        mqttClient.subscribe(mqtt_HassIO_time_command);             // HassIO sends a time update every 30 sec
        mqttClient.subscribe(mqtt_HassIO_sun_command);              // HassIO sends a 1 or 0 when the sun is rising or falling
        mqttClient.subscribe(mqtt_HassIO_master_present_command);   // HassIO sends a 1 or 0 when the master is present or not
#ifdef CONTROLLER_MOTION_DISABLE_WHEN_PC_ON
        mqttClient.subscribe(mqtt_HassIO_pc_present_command);       // HassIO sends a 1 or 0 when the pc is on or off
#endif

        //*** General ***//
        //-- Motion Detection
        mqttClient.subscribe(mqtt_motion_detection_power_command);
        mqttClient.subscribe(mqtt_motion_detection_rgb_command);
        mqttClient.subscribe(mqtt_motion_detection_timeout_command);


        //*** Specific ***//
        //---- LED Strip 1
        mqttClient.subscribe(mqtt_strip1_power_command);
        //-- Check if Brightness is needed
        if (HardwareConfigStrip1.isRGB or HardwareConfigStrip1.isCW or HardwareConfigStrip1.isWW) {
          mqttClient.subscribe(mqtt_strip1_brightness_command);
        }
        //-- Check if RGB is supported
        if (HardwareConfigStrip1.isRGB) {
          mqttClient.subscribe(mqtt_strip1_rgb_command);
        }
        //-- Check if CW is supported
        if (HardwareConfigStrip1.isCW) {
          mqttClient.subscribe(mqtt_strip1_cold_white_value_command);
        }
        //-- Check if WW is supported
        if (HardwareConfigStrip1.isWW) {
          mqttClient.subscribe(mqtt_strip1_warm_white_value_command);
        }
        mqttClient.subscribe(mqtt_strip1_effect_command);

        //---- LED Strip 2
        mqttClient.subscribe(mqtt_strip2_power_command);
        //-- Check if Brightness is needed
        if (HardwareConfigStrip2.isRGB or HardwareConfigStrip2.isCW or HardwareConfigStrip2.isWW) {
          mqttClient.subscribe(mqtt_strip2_brightness_command);
        }
        //-- Check if RGB is supported
        if (HardwareConfigStrip2.isRGB) {
          mqttClient.subscribe(mqtt_strip2_rgb_command);
        }
        //-- Check if CW is supported
        if (HardwareConfigStrip2.isCW) {
          mqttClient.subscribe(mqtt_strip2_cold_white_value_command);
        }
        //-- Check if WW is supported
        if (HardwareConfigStrip2.isWW) {
          mqttClient.subscribe(mqtt_strip2_warm_white_value_command);
        }
        mqttClient.subscribe(mqtt_strip2_effect_command);


        //-- LED Effect Modes

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
    HassIOTimeout                 = true;
    SendMqttParameter             = true;
    NetworkState.HassIO_Connected = false;
  } else {
    HassIOTimeout                 = false;
    NetworkState.HassIO_Connected = true;
  }

  //-- If HassIO is avaiable again send data
  if (SendMqttParameter and !HassIOTimeout) {
    SendMqttParameter = false;
    char tempValueHolder[32];

    //-- LED Strip 1
    sprintf(tempValueHolder, "%d",  ParameterLEDStrip1.Power);
    mqttClient.publish(mqtt_strip1_power_state, tempValueHolder);
    //-- Check if Brightness is needed
    if (HardwareConfigStrip1.isRGB or HardwareConfigStrip1.isCW or HardwareConfigStrip1.isWW) {
      sprintf(tempValueHolder, "%d",  ParameterLEDStrip1.Brightness);
      mqttClient.publish(mqtt_strip1_brightness_state, tempValueHolder);
    }
    //-- Check if RGB is supported
    if (HardwareConfigStrip1.isRGB) {
      mqttClient.publish(mqtt_strip1_rgb_state, LastColorStrip1Holder);
    }
    //-- Check if CW is supported
    if (HardwareConfigStrip1.isCW) {
      sprintf(tempValueHolder, "%d",  ParameterLEDStrip1.ColdWhite);
      mqttClient.publish(mqtt_strip1_cold_white_value_state, tempValueHolder);
    }
    //-- Check if WW is supported
    if (HardwareConfigStrip1.isWW) {
      sprintf(tempValueHolder, "%d",  ParameterLEDStrip1.WarmWhite);
      mqttClient.publish(mqtt_strip1_warm_white_value_state, tempValueHolder);
    }
    mqttClient.publish(mqtt_strip1_effect_state, LastEffectStrip1Holder);

    //-- LED Strip 2
    sprintf(tempValueHolder, "%d",  ParameterLEDStrip2.Power);
    mqttClient.publish(mqtt_strip2_power_state, tempValueHolder);
    //-- Check if Brightness is needed
    if (HardwareConfigStrip2.isRGB or HardwareConfigStrip2.isCW or HardwareConfigStrip2.isWW) {
      sprintf(tempValueHolder, "%d",  ParameterLEDStrip2.Brightness);
      mqttClient.publish(mqtt_strip2_brightness_state, tempValueHolder);
    }
    //-- Check if RGB is supported
    if (HardwareConfigStrip2.isRGB) {
      mqttClient.publish(mqtt_strip2_rgb_state, LastColorStrip2Holder);
    }
    //-- Check if CW is supported
    if (HardwareConfigStrip1.isCW) {
      sprintf(tempValueHolder, "%d",  ParameterLEDStrip2.ColdWhite);
      mqttClient.publish(mqtt_strip2_cold_white_value_state, tempValueHolder);
    }
    //-- Check if WW is supported
    if (HardwareConfigStrip2.isWW) {
      sprintf(tempValueHolder, "%d",  ParameterLEDStrip2.WarmWhite);
      mqttClient.publish(mqtt_strip2_warm_white_value_state, tempValueHolder);
    }
    mqttClient.publish(mqtt_strip2_effect_state, LastEffectStrip2Holder);
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
