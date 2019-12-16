

void wifi() {

  // Get current millis fro WiFi reconnect Timeout
  unsigned long CurMillis_NoWiFiConnected = millis();

  switch (WiFi_State) {

    //-- Try Connecting to WiFi
    case 0:
      Serial.println("Start WiFi Connection");
      WiFi.mode(WIFI_STA);
      WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
      PrevMillis_NoWiFiConnected = CurMillis_NoWiFiConnected; // Set prev_time for connection timeout
      WiFi_State = 1; // Wait for WiFi to connect
      break;

    //-- Check if WiFi is Connected
    case 1:
      // Check if WiFI is connected
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("  WiFi Connected");
        Serial.println("");
        WiFi_State = 10; // Check if WiFi disconnects
      }
      // Wait 5 seconds for the WiFI to connect after that try again
      if (CurMillis_No_WiFi_Connected - PrevMillis_No_WiFi_Connected >= TimeOut_No_WiFi_Connected) {
        Serial.println("  Connecting WiFi Failed!!");
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
        WiFI_State = 0; // When disconnected reconnect
      }
      break;
  }

}

void mqtt() {

  switch (MQTT_State) {

    //-- Check if WiFI is connected
    case 0:

      break;

  }


  if (!mqtt_Client.connected()) {

    if (mqtt_Client.connect(MQTT_CLIENT_NAME, MQTT_USERNAME, MQTT_PASSWORD)) {
      Serial.println("Start channel subscription");

      //*** System ***//
      mqtt_Client.subscribe( mqtt_command_System_Reboot_Specific );   //Reboot command path for this specific controller
      mqtt_Client.subscribe( mqtt_command_System_Reboot );            //Reboot command path for all used controller connected to the broker

      //*** Other ***//

      Serial.println("Finished channel subscription");
      Serial.println("");
    }
  }

}


//------------------------------------- MQTT Callback -------------------------------------//
void callback(char* topic, byte * payload, unsigned int length) {

  //-- Get Message and add terminator
  char message[length + 1];
  for (int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0';

  //######################################## System ########################################//
  //------------------- Parameter [mqtt_System_Reboot] -------------------//
  if ((String(mqtt_command_System_Reboot_Specific).equals(topic)) or (String(mqtt_command_System_Reboot).equals(topic))) {
    uint8_t data = atoi(message);
    if (data == 1) {
      mqtt_System_Reboot = true;
    }
    PrevMillis_SystemRebootDelay = millis();
  }

}


void heartbeat() {

  //-- Send heartbeat over MQTT
  unsigned long CurMillis_HeartBeat = millis();
  if (CurMillis_HeartBeat - PrevMillis_HeartBeat >= TimeOut_HeartBeat) {
    PrevMillis_HeartBeat = CurMillis_HeartBeat;
    char message[20];
    sprintf(message, "%ld", HeartBeatCounter);
    mqtt_Client.publish(mqtt_state_Heartbeat, message);
    HeartBeatCounter++;
  }

}
