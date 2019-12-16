

void wifi() {

  // Get current millis for WiFi reconnect Timeout
  unsigned long CurMillis_No_WiFi_Connection = millis();

  switch (WiFi_State) {

    //-- Try Connecting to WiFi
    case 0:
      Serial.println("Start WiFi Connection");
      WiFi.mode(WIFI_STA);
      WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
      PrevMillis_No_WiFi_Connection = CurMillis_No_WiFi_Connection; // Set prev_time for connection timeout
      WiFi_State = 1; // Wait for WiFi to connect
      break;


    //-- Check if WiFi is Connected
    case 1:
      // Check if WiFI is connected
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("  WiFi Connected!");
        Serial.println("");
        WiFi_State = 10; // Check if WiFi disconnects
      }
      // Wait 5 seconds for the WiFI to connect after that try again
      if (CurMillis_No_WiFi_Connection - PrevMillis_No_WiFi_Connection >= TimeOut_No_WiFi_Connection) {
        Serial.println("  Connecting WiFi Failed!");
        Serial.println("");
        WiFi_State = 2; // Disconnect WiFi
      }
      break;


    //-- Disconnect WiFI
    case 2:
      WiFi.disconnect();
      WiFi_State = 0; // Restart WiFi connection
      break;


    //-- Check if WiFI is Disctonnected
    case 10:
      if (WiFi.status() != WL_CONNECTED) {
        WiFi_State = 0; // When disconnected reconnect
      }
      break;


    //-- If something crashes start again
    default:
      WiFi_State = 0;
      break;
  }

}


void mqtt() {

  // Get current millis for MQTT reconnect Timeout
  unsigned long CurMillis_No_MQTT_Connection = millis();

  switch (MQTT_State) {

    //-- Check if WiFi is connected and the client isn't connected to the MQTT broker
    case 0:
      if ((WiFi.status() == WL_CONNECTED) and !mqtt_Client.connected()) {
        Serial.println("Start MQTT Connection");
        MQTT_State = 1; // Try connecting to the MQTT broker
      }
      break;


    //-- Try connecting to the MQTT broker as long as the WiFi is avaiable
    case 1:
      if (WiFi.status() == WL_CONNECTED) {
        mqtt_Client.connect(MQTT_CLIENT_NAME, MQTT_USERNAME, MQTT_PASSWORD);
        PrevMillis_No_MQTT_Connection = CurMillis_No_MQTT_Connection; // Set prev_time for connection timeout
        MQTT_State = 2;
      } else {
        MQTT_State = 0; // Check if WiFi is avaiable and client is disconnected
      }
      break;


    //-- If connected to the WiFi and the broker subscribe to the channel. If not after 5 seconds try again
    case 2:
      if ((WiFi.status() == WL_CONNECTED) and mqtt_Client.connected()) {

        Serial.println("  MQTT Connected!");

        //*** General ***//
        mqtt_Client.subscribe(mqtt_command_HassIO_Heartbeat);    // HassIO sends a Heartbeat every second

        //*** Specific ***//
        mqtt_Client.subscribe(mqtt_led_strip_1_json_command);    // HassIO commands for led strip 1
        mqtt_Client.subscribe(mqtt_led_strip_2_json_command);    // HassIO commands for led strip 1

        MQTT_State = 0;
      } else {
        // Wait 5 seconds to connect to the MQTT broker after that try again
        if (CurMillis_No_MQTT_Connection - PrevMillis_No_MQTT_Connection >= TimeOut_No_MQTT_Connection) {
          Serial.println("  Connecting to MQTT Failed!");
          Serial.println("");
          MQTT_State = 0; // Check if WiFi is aviable and client is disconnected
        }
      }
      break;


    //-- If something crashes start again
    default:
      MQTT_State = 0;
      break;

  }

}


void heartbeat() {

  //-- Send heartbeat over MQTT
  unsigned long CurMillis_HeartBeat = millis();
  if (CurMillis_HeartBeat - PrevMillis_HeartBeat >= TimeOut_HeartBeat) {
    PrevMillis_HeartBeat = CurMillis_HeartBeat;
    char message[20];
    sprintf(message, "%ld", ESP_Heart_Beat_Counter);
    mqtt_Client.publish(mqtt_state_ESP_Heartbeat, message);
    ESP_Heart_Beat_Counter++;
  }

}
