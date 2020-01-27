

void leds() {

  //-- Call Control Function
  StripControl(&HardwareConfigStrip1, &CurStrip1Data, &NextStrip1Data, &TransitionDataStrip1, &ParameterLEDStrip1, &EffectDataListStrip1);
  StripControl(&HardwareConfigStrip2, &CurStrip2Data, &NextStrip2Data, &TransitionDataStrip2, &ParameterLEDStrip2, &EffectDataListStrip2);

  //-- Call Fade Function
  FadeStrip(&HardwareConfigStrip1, &CurStrip1Data, &NextStrip1Data, &FadeDataStrip1, &TransitionDataStrip1);
  FadeStrip(&HardwareConfigStrip2, &CurStrip2Data, &NextStrip2Data, &FadeDataStrip2, &TransitionDataStrip2);

  //-- Call Show Function
  ShowStrip(&HardwareConfigStrip1, &CurStrip1Data);
  ShowStrip(&HardwareConfigStrip2, &CurStrip2Data);

}


void StripControl(HardwareStripConfig   *HardwareStrip,
                  StripData             *CurData,
                  StripData             *NextData,
                  TransitionData        *TransData,
                  StripParameter        *Strip,
                  EffectDataList        *EffectData) {

  //-- When Master is not present turn off strips
  if (!ParameterHassIO.MasterPresent) {
    NextData->Brightness = 0;
    return;
  }

  //-- Commands for mqtt publish
  char off_cmd[]    = "0";
  char on_cmd[]     = "1";
  char e_none_cmd[] = "None";

  //-- Mode Control
  /*
     Prio from bottom highest to lowest at the top.
     Mode can only be changed when in idle
  */
  if (TransData->Finished) {

    //---- Idle
    // PRIO Reset
    if (TransData->CurMode != Idle) {
      TransData->NextMode = Idle;
    }

    //---- Motion
    // PRIO 3
    if (
      // PRIO Reset
      TransData->CurMode == Idle      or
      // PRIO 3
      TransData->CurMode == Motion
    ) {

      //-- Check if CurMode is Idle or unequal Alarm if so reset
      if (TransData->CurMode == Idle or TransData->CurMode != Motion) {
        //-- Reset
        ResetEffectData(&(EffectData->Motion));
        MotionData.MotionEffectActive = false;
      }

      //-- Check if Motion has been deteced or if the effect is still going on. Only works when motion detection is enabled
      if (
        (
          MotionData.MotionDeteced      or
          MotionData.MotionEffectActive
        )
        and ParameterMotion.Power
        and ParameterHassIO.SunBelowHorizon
#ifdef CONTROLLER_MOTION_DISABLE_WHEN_PC_ON
        and !ParameterHassIO.PcPowerdOn
#endif
      ) {

        //-- Set new Mode
        TransData->NextMode = Motion;

        //-- Start Effect sequence
        switch (EffectData->Motion.State) {

          case 0: //-- Set Effect Start Parameter and wait for the Transition to end
            //-- Set Start Parameter
            NextData->R           = ParameterMotion.Red;
            NextData->G           = ParameterMotion.Green;
            NextData->B           = ParameterMotion.Blue;
            NextData->CW          = 0;
            NextData->WW          = 0;
            NextData->Brightness  = getTimeBasedBrightness();
            //-- Wait for Transition to finish
            if (TransData->Finished and (TransData->CurMode == TransData->NextMode)) {
              EffectData->Motion.PrevMillis = millis();
              EffectData->Motion.State      = 1;
            }
            //-- Set Effect activ flag
            MotionData.MotionEffectActive = true;
            break;

          case 1: //-- Run Effect

            //-- Check if Motion is retriggert
            if (MotionData.MotionDeteced) {
              EffectData->Motion.PrevMillis = millis(); //-- Reset Timer
            }

            //-- Timer waits to run out and stop motion effect
            if ((millis() - EffectData->Motion.PrevMillis) >= (ParameterMotion.Timeout * 1000)) {
              MotionData.MotionEffectActive = false;
            }
            break;

        }

      } else {
        NextData->Brightness = 0;  //-- Power Off
      }

    }

    //---- Effects
    // PRIO 2
    if (
      // PRIO Reset
      TransData->CurMode == Idle      or
      // PRIO 3
      TransData->CurMode == Motion    or
      // PRIO 2
      TransData->CurMode == Normal    or
      TransData->CurMode == Alarm     or
      TransData->CurMode == Wakeup    or
      TransData->CurMode == Sleep     or
      TransData->CurMode == Weekend
    ) {

      if (Strip->Power or Strip->Effect == e_Wakeup or Strip->Effect == e_Alarm) {
        switch (Strip->Effect) {

          //-- None / Normal mode
          case e_None:
          case e_Only_RGB:
          case e_Only_CW:
          case e_Only_WW:
          case e_Only_RGBCW:
          case e_Only_RGBWW:

            //-- Set new Mode
            TransData->NextMode = Normal;
            //-- Set Mode Parameter
            //-- Check if RGB is supported
            if (HardwareStrip->isRGB and (Strip->Effect == e_None or Strip->Effect == e_Only_RGB or Strip->Effect == e_Only_RGBCW or Strip->Effect == e_Only_RGBWW)) {
              NextData->R = Strip->Red;
              NextData->G = Strip->Green;
              NextData->B = Strip->Blue;
            } else {
              NextData->R  = 0;
              NextData->G  = 0;
              NextData->B  = 0;
            }

            //-- Check if CW is supported
            if (HardwareStrip->isCW and (Strip->Effect == e_None or Strip->Effect == e_Only_CW or Strip->Effect == e_Only_RGBCW)) {
              NextData->CW = Strip->ColdWhite;
            } else {
              NextData->CW  = 0;
            }

            //-- Check if WW is supported
            if (HardwareStrip->isWW and (Strip->Effect == e_None or Strip->Effect == e_Only_WW or Strip->Effect == e_Only_RGBWW)) {
              NextData->WW = Strip->WarmWhite;
            } else {
              NextData->WW  = 0;
            }

            //-- Check if Brightness is used
            if (HardwareStrip->isRGB or HardwareStrip->isCW or HardwareStrip->isWW) {
              NextData->Brightness = Strip->Brightness;
            }
            break;

          //-- Alarm Mode
          case e_Alarm:

            //-- Check if CurMode is Idle or unequal Alarm if so reset
            if (TransData->CurMode == Idle or TransData->CurMode != Alarm) {
              //-- Reset
              ResetEffectData(&(EffectData->Alarm));
            }

            //-- Set new Mode
            TransData->NextMode = Alarm;

            //-- Start Effect sequence
            switch (EffectData->Alarm.State) {

              case 0: //-- Set Effect Start Parameter and wait for the Transition to end
                //-- Set Start Parameter
                NextData->R           = MAX_COLOR_VALUE;
                NextData->G           = 0;
                NextData->B           = 0;
                NextData->CW          = 0;
                NextData->WW          = 0;
                NextData->Brightness  = MAX_BRIGHTNESS_VALUE;
                //-- Wait for Transition to finish
                if (TransData->Finished and (TransData->CurMode == TransData->NextMode)) {
                  EffectData->Alarm.PrevMillis = millis();
                  EffectData->Alarm.State      = 1;
                }
                break;

              case 1: //-- Run Effect
                if ((millis() - EffectData->Alarm.PrevMillis) >= EffectData->Alarm.Timeout) {

                  switch (EffectData->Alarm.SubState) {

                    case 0: //-- Fade Brightness to 0
                      NextData->Brightness = 0;
                      if (CurData->Brightness == NextData->Brightness) {
                        EffectData->Alarm.SubState = 1;
                      }
                      break;

                    case 1: //-- Wait 100 * Alarm.Timeout
                      if (EffectData->Alarm.SubCounter == 100) {
                        EffectData->Alarm.SubCounter = 0;
                        EffectData->Alarm.SubState   = 2;
                      } else {
                        EffectData->Alarm.SubCounter++;
                      }
                      break;

                    case 2: //-- Fade Brightness to Max
                      NextData->Brightness = MAX_BRIGHTNESS_VALUE;
                      if (CurData->Brightness == NextData->Brightness) {
                        EffectData->Alarm.SubState = 3;
                      }
                      break;

                    case 3: //-- Wait 50 * Alarm.Timeout => Restart
                      if (EffectData->Alarm.SubCounter == 50) {
                        EffectData->Alarm.SubCounter = 0;  //-- Reset
                        EffectData->Alarm.SubState   = 0;  //-- Reset
                        EffectData->Alarm.Counter    = 0;  //-- Reset
                      } else {
                        EffectData->Alarm.SubCounter++;
                      }
                      break;

                  }

                }

            }

            break;

          //-- WakeUp Mode
          case e_Wakeup:

            //-- Check if CurMode is Idle or unequal Alarm if so reset
            if (TransData->CurMode == Idle or TransData->CurMode != Wakeup) {
              //-- Reset
              ResetEffectData(&(EffectData->Wakeup));
            }

            //-- Set new Mode
            TransData->NextMode = Wakeup;

            //-- Start Effect sequence
            switch (EffectData->Wakeup.State) {

              case 0: //-- Set Effect Start Parameter and wait for the Transition to end
                //-- Set Start Parameter
                NextData->R           = MAX_COLOR_VALUE;
                NextData->G           = int(MAX_COLOR_VALUE / 4);
                NextData->B           = 0;
                NextData->CW          = 0;
                NextData->WW          = 0;
                CurData->Brightness   = 0;
                //-- Wait for Transition to finish
                if (TransData->Finished and (TransData->CurMode == TransData->NextMode)) {
                  EffectData->Wakeup.PrevMillis = millis();
                  EffectData->Wakeup.State      = 1;
                }
                break;

              case 1: //-- Run Effect

                if ((millis() - EffectData->Wakeup.PrevMillis) >= EffectData->Wakeup.Timeout * 50) {

                  EffectData->Wakeup.PrevMillis = millis();

                  switch (EffectData->Wakeup.SubState) {

                    case 0: //-- Fade Brightness to Max
                      if (CurData->Brightness == MAX_BRIGHTNESS_VALUE) {
                        EffectData->Wakeup.SubState++;
                      } else {
                        if (NextData->Brightness < MAX_BRIGHTNESS_VALUE) {
                          NextData->Brightness += 1;
                        }
                      }
                      break;

                    case 1: //-- Turn on Strip
                      switch (HardwareStrip->StripID) {

                        case 1:
                          mqttClient.publish(mqtt_strip1_power_command, on_cmd);
                          mqttClient.publish(mqtt_strip1_effect_command, e_none_cmd);
                          break;

                        case 2:
                          mqttClient.publish(mqtt_strip2_power_command, on_cmd );
                          mqttClient.publish(mqtt_strip2_effect_command, e_none_cmd);
                          break;
                      }
                      EffectData->Wakeup.SubState++;
                      break;

                    case 2: //-- Dummy Wait

                      break;

                  }

                }

            }
            break;

          //-- Sleep Mode
          case e_Sleep:

            //-- Check if CurMode is Idle or unequal Alarm if so reset
            if (TransData->CurMode == Idle or TransData->CurMode != Sleep) {
              //-- Reset
              ResetEffectData(&(EffectData->Sleep));
            }

            //-- Set new Mode
            TransData->NextMode = Sleep;

            //-- Start Effect sequence
            switch (EffectData->Sleep.State) {

              case 0: //-- Set Effect Start Parameter and wait for the Transition to end
                //-- Set Start Parameter
                NextData->R           = MAX_COLOR_VALUE;
                NextData->G           = int(MAX_COLOR_VALUE / 4);
                NextData->B           = 0;
                NextData->CW          = 0;
                NextData->WW          = 0;
                //-- Wait for Transition to finish
                if (TransData->Finished and (TransData->CurMode == TransData->NextMode)) {
                  EffectData->Sleep.PrevMillis = millis();
                  EffectData->Sleep.State      = 1;
                }
                break;

              case 1: //-- Run Effect

                if ((millis() - EffectData->Sleep.PrevMillis) >= EffectData->Sleep.Timeout * 20) {

                  EffectData->Sleep.PrevMillis = millis();

                  switch (EffectData->Sleep.SubState) {

                    case 0: //-- Fade Brightness to 0
                      if (CurData->Brightness == 0) {
                        EffectData->Sleep.SubState++;
                      } else {
                        if (NextData->Brightness > 0) {
                          NextData->Brightness -= 1;
                        }
                      }
                      break;

                    case 1: //-- Turn off Strip
                      switch (HardwareStrip->StripID) {

                        case 1:
                          mqttClient.publish(mqtt_strip1_power_command, off_cmd);
                          mqttClient.publish(mqtt_strip1_effect_command, e_none_cmd);
                          break;

                        case 2:
                          mqttClient.publish(mqtt_strip2_power_command, off_cmd );
                          mqttClient.publish(mqtt_strip2_effect_command, e_none_cmd);
                          break;
                      }
                      EffectData->Sleep.SubState++;
                      break;

                    case 2: //-- Dummy Wait

                      break;

                  }

                }

            }
            break;

          //-- Weekend Mode
          case e_Weekend:

            //-- Check if CurMode is Idle or unequal Alarm if so reset
            if (TransData->CurMode == Idle or TransData->CurMode != Weekend) {
              //-- Reset
              ResetEffectData(&(EffectData->Weekend));
            }

            //-- Set new Mode
            TransData->NextMode = Weekend;

            //-- Start Effect sequence
            switch (EffectData->Weekend.State) {

              case 0: //-- Set Effect Start Parameter and wait for the Transition to end
                //-- Set Start Parameter
                NextData->R           = MAX_COLOR_VALUE;
                NextData->G           = MAX_COLOR_VALUE;
                NextData->B           = 0;
                NextData->CW          = 0;
                NextData->WW          = 0;
                NextData->Brightness  = MAX_BRIGHTNESS_VALUE;
                //-- Wait for Transition to finish
                if (TransData->Finished and (TransData->CurMode == TransData->NextMode)) {
                  EffectData->Weekend.PrevMillis = millis();
                  EffectData->Weekend.State      = 1;
                }
                break;

              case 1: //-- Run Effect
                if ((millis() - EffectData->Weekend.PrevMillis) >= (EffectData->Weekend.Timeout * 10)) {

                  EffectData->Weekend.PrevMillis = millis();

                  switch (EffectData->Weekend.SubState) {

                    case 0:
                      NextData->R -= 5;
                      if (NextData->R == 0) {
                        EffectData->Weekend.SubState++;
                      }
                      break;

                    case 1:
                      NextData->B += 5;
                      if (NextData->B == 255) {
                        EffectData->Weekend.SubState++;
                      }
                      break;

                    case 2:
                      NextData->G -= 5;
                      if (NextData->G == 0) {
                        EffectData->Weekend.SubState++;
                      }
                      break;

                    case 3:
                      NextData->R += 5;
                      if (NextData->R == 255) {
                        EffectData->Weekend.SubState++;
                      }
                      break;

                    case 4:
                      NextData->B -= 5;
                      if (NextData->B == 0) {
                        EffectData->Weekend.SubState++;
                      }
                      break;

                    case 5:
                      NextData->G += 5;
                      if (NextData->G == 255) {
                        EffectData->Weekend.SubState = 0;
                      }
                      break;

                  }

                }

            }

            break;

        }
      } else {
        if (TransData->CurMode != Motion) {
          NextData->Brightness = 0;  //-- Power Off
        }
      }
    }

    //---- Status Messages
    // PRIO 1
    //-- NoHassIO
    if (
      // PRIO Reset
      TransData->CurMode == Idle      or
      // PRIO 3
      TransData->CurMode == Motion    or
      // PRIO 2
      TransData->CurMode == Normal    or
      TransData->CurMode == Alarm     or
      TransData->CurMode == Wakeup    or
      TransData->CurMode == Sleep     or
      TransData->CurMode == Weekend   or
      // PRIO 1
      TransData->CurMode == NoHassIO  or
      TransData->CurMode == NoMQTT    or
      TransData->CurMode == NoWiFi
    ) {

      if (!NetworkState.HassIO_Connected and !DisablePrio3Effect) {

        //-- Check if CurMode is Idle or unequal NoHassIO if so reset
        if (TransData->CurMode == Idle or TransData->CurMode != NoHassIO) {
          //-- Reset
          ResetEffectData(&(EffectData->NoHassIO));
        }

        //-- Set new Mode
        TransData->NextMode = NoHassIO;

        //-- Start Effect sequence
        switch (EffectData->NoHassIO.State) {

          case 0: //-- Set Effect Start Parameter and wait for the Transition to end
            //-- Set Start Parameter
            if (TransData->CurMode != NoMQTT and TransData->CurMode != NoWiFi) {
              NextData->R           = 0;
              NextData->G           = MAX_COLOR_VALUE;
              NextData->B           = MAX_COLOR_VALUE;
              NextData->CW          = 0;
              NextData->WW          = 0;
              NextData->Brightness  = MAX_BRIGHTNESS_VALUE;
            }
            //-- Wait for Transition to finish
            if (TransData->Finished and (TransData->CurMode == TransData->NextMode)) {
              EffectData->NoHassIO.PrevMillis = millis();
              EffectData->NoHassIO.State      = 1;
            }
            break;

          case 1: //-- Run Effect
            if (EffectData->NoHassIO.Counter <= 3) {
              if ((millis() - EffectData->NoHassIO.PrevMillis) >= EffectData->NoHassIO.Timeout) {

                switch (EffectData->NoHassIO.SubState) {

                  case 0: //-- Fade Brightness to 0
                    NextData->Brightness = 0;
                    if (CurData->Brightness == NextData->Brightness) {
                      EffectData->NoHassIO.SubState = 1;
                    }
                    break;

                  case 1: //-- Wait 100 * NoHassIO.Timeout
                    if (EffectData->NoHassIO.SubCounter == 100) {
                      EffectData->NoHassIO.SubCounter = 0;
                      EffectData->NoHassIO.SubState   = 2;
                    } else {
                      EffectData->NoHassIO.SubCounter++;
                    }
                    break;

                  case 2: //-- Fade Brightness to Max
                    NextData->Brightness = MAX_BRIGHTNESS_VALUE;
                    if (CurData->Brightness == NextData->Brightness) {
                      EffectData->NoHassIO.SubState = 3;
                    }
                    break;

                  case 3: //-- Wait 50 * NoHassIO.Timeout => Restart
                    if (EffectData->NoHassIO.SubCounter == 50) {
                      EffectData->NoHassIO.SubCounter = 0;  //-- Reset
                      EffectData->NoHassIO.SubState   = 0;  //-- Reset
                      EffectData->NoHassIO.Counter++;       //-- Count
                    } else {
                      EffectData->NoHassIO.SubCounter++;
                    }
                    break;

                }

              }
            } else {
              EffectData->NoHassIO.State = 2;
            }
            break;

          case 2: //-- Never ending wait until connection is established again
            NextData->Brightness = 0; //-- Power Off Strip
            break;

        }

      }

      //-- NoMQTT
      if (!NetworkState.MQTT_Connected and !DisablePrio3Effect) {

        //-- Check if CurMode is Idle or unequal NoMQTT if so reset
        if (TransData->CurMode == Idle or TransData->CurMode != NoMQTT) {
          //-- Reset
          ResetEffectData(&(EffectData->NoMQTT));
        }

        //-- Set new Mode
        TransData->NextMode = NoMQTT;

        //-- Start Effect sequence
        switch (EffectData->NoMQTT.State) {

          case 0: //-- Set Effect Start Parameter and wait for the Transition to end
            //-- Set Start Parameter
            if (TransData->CurMode != NoHassIO and TransData->CurMode != NoWiFi) {
              NextData->R           = 0;
              NextData->G           = 0;
              NextData->B           = MAX_COLOR_VALUE;
              NextData->CW          = 0;
              NextData->WW          = 0;
              NextData->Brightness  = MAX_BRIGHTNESS_VALUE;
            }
            //-- Wait for Transition to finish
            if (TransData->Finished and (TransData->CurMode == TransData->NextMode)) {
              EffectData->NoMQTT.PrevMillis = millis();
              EffectData->NoMQTT.State      = 1;
            }
            break;

          case 1: //-- Run Effect
            if (EffectData->NoMQTT.Counter <= 3) {
              if ((millis() - EffectData->NoMQTT.PrevMillis) >= EffectData->NoMQTT.Timeout) {

                switch (EffectData->NoMQTT.SubState) {

                  case 0: //-- Fade Brightness to 0
                    NextData->Brightness = 0;
                    if (CurData->Brightness == NextData->Brightness) {
                      EffectData->NoMQTT.SubState = 1;
                    }
                    break;

                  case 1: //-- Wait 100 * NoMQTT.Timeout
                    if (EffectData->NoMQTT.SubCounter == 100) {
                      EffectData->NoMQTT.SubCounter = 0;
                      EffectData->NoMQTT.SubState   = 2;
                    } else {
                      EffectData->NoMQTT.SubCounter++;
                    }
                    break;

                  case 2: //-- Fade Brightness to Max
                    NextData->Brightness = MAX_BRIGHTNESS_VALUE;
                    if (CurData->Brightness == NextData->Brightness) {
                      EffectData->NoMQTT.SubState = 3;
                    }
                    break;

                  case 3: //-- Wait 50 * NoMQTT.Timeout => Restart
                    if (EffectData->NoMQTT.SubCounter == 50) {
                      EffectData->NoMQTT.SubCounter = 0;  //-- Reset
                      EffectData->NoMQTT.SubState   = 0;  //-- Reset
                      EffectData->NoMQTT.Counter++;       //-- Count
                    } else {
                      EffectData->NoMQTT.SubCounter++;
                    }
                    break;

                }

              }
            } else {
              EffectData->NoMQTT.State = 2;
            }
            break;

          case 2: //-- Never ending wait until connection is established again
            NextData->Brightness = 0; //-- Power Off Strip
            break;

        }

      }

      //-- NoWiFi
      if (!NetworkState.Wifi_Connected and !DisablePrio3Effect) {

        //-- Check if CurMode is Idle or unequal NoWiFi if so reset
        if (TransData->CurMode == Idle or TransData->CurMode != NoWiFi) {
          //-- Reset
          ResetEffectData(&(EffectData->NoWiFi));
        }

        //-- Set new Mode
        TransData->NextMode = NoWiFi;

        //-- Start Effect sequence
        switch (EffectData->NoWiFi.State) {

          case 0: //-- Set Effect Start Parameter and wait for the Transition to end
            //-- Set Start Parameter
            if (TransData->CurMode != NoHassIO and TransData->CurMode != NoMQTT) {
              NextData->R           = 0;
              NextData->G           = MAX_COLOR_VALUE;
              NextData->B           = 0;
              NextData->CW          = 0;
              NextData->WW          = 0;
              NextData->Brightness  = MAX_BRIGHTNESS_VALUE;
            }
            //-- Wait for Transition to finish
            if (TransData->Finished and (TransData->CurMode == TransData->NextMode)) {
              EffectData->NoWiFi.PrevMillis = millis();
              EffectData->NoWiFi.State      = 1;
            }
            break;

          case 1: //-- Run Effect
            if (EffectData->NoWiFi.Counter <= 3) {
              if ((millis() - EffectData->NoWiFi.PrevMillis) >= EffectData->NoWiFi.Timeout) {

                switch (EffectData->NoWiFi.SubState) {

                  case 0: //-- Fade Brightness to 0
                    NextData->Brightness = 0;
                    if (CurData->Brightness == NextData->Brightness) {
                      EffectData->NoWiFi.SubState = 1;
                    }
                    break;

                  case 1: //-- Wait 100 * NoWiFi.Timeout
                    if (EffectData->NoWiFi.SubCounter == 100) {
                      EffectData->NoWiFi.SubCounter = 0;
                      EffectData->NoWiFi.SubState   = 2;
                    } else {
                      EffectData->NoWiFi.SubCounter++;
                    }
                    break;

                  case 2: //-- Fade Brightness to Max
                    NextData->Brightness = MAX_BRIGHTNESS_VALUE;
                    if (CurData->Brightness == NextData->Brightness) {
                      EffectData->NoWiFi.SubState = 3;
                    }
                    break;

                  case 3: //-- Wait 50 * NoWiFi.Timeout => Restart
                    if (EffectData->NoWiFi.SubCounter == 50) {
                      EffectData->NoWiFi.SubCounter = 0;  //-- Reset
                      EffectData->NoWiFi.SubState   = 0;  //-- Reset
                      EffectData->NoWiFi.Counter++;       //-- Count
                    } else {
                      EffectData->NoWiFi.SubCounter++;
                    }
                    break;

                }

              }
            } else {
              EffectData->NoWiFi.State = 2;
            }
            break;

          case 2: //-- Never ending wait until connection is established again
            NextData->Brightness = 0; //-- Power Off Strip
            break;

        }

      }

    }

  }

  //-- Call Mode Transition
  ModeTransition(TransData, CurData, NextData, HardwareStrip);

}


void ModeTransition(TransitionData        * TransData,
                    StripData             * CurData,
                    StripData             * NextData,
                    HardwareStripConfig   * HardwareStrip) {

  switch (TransData->State) {

    //-- Idle
    case 0:
      if (TransData->CurMode != TransData->NextMode) {
        TransData->BackUpBrightness  = NextData->Brightness;   //Backup NextData->Brightness
        TransData->Finished          = false;
        TransData->State             = 1;
      }
      break;

    //-- Brightness to zero
    case 1:
      NextData->Brightness = 0;
      if (CurData->Brightness == 0) {
        TransData->State = 2;
      }
      break;

    //-- Set Color of new mode
    case 2:
      //-- Check if RGB is supported
      if (HardwareStrip->isRGB) {
        CurData->R = NextData->R; //-- Direct Overwrite => No Fade
        CurData->G = NextData->G; //-- Direct Overwrite => No Fade
        CurData->B = NextData->B; //-- Direct Overwrite => No Fade
      }
      //-- Check if CW is supported
      if (HardwareStrip->isCW) {
        CurData->CW = NextData->CW; //-- Direct Overwrite => No Fade
      }
      //-- Check if WW is supported
      if (HardwareStrip->isWW) {
        CurData->WW = NextData->WW; //-- Direct Overwrite => No Fade
      }
      //Revert NextData->Brightness
      NextData->Brightness = TransData->BackUpBrightness;
      TransData->State = 3;
      break;

    //-- Brightness to next Mode
    case 3:
      if (CurData->Brightness == NextData->Brightness) {
        TransData->Finished   = true;
        TransData->CurMode    = TransData->NextMode;
        TransData->State      = 0;
      }
      break;

  }

}


void FadeStrip(HardwareStripConfig  * HardwareStrip,
               StripData            * CurData,
               StripData            * NextData,
               FadeData             * Fade,
               TransitionData       * TransData) {

  //---- Check Data for miss configuration
  //-- Fade Step Color
  if (Fade->FadeStepColor == 0 or Fade->FadeStepColor > 50) {
    Fade->FadeStepColor = 5; //-- Init Value
  }

  //-- Fade Step Brightness
  if (Fade->FadeStepBrightness == 0 or Fade->FadeStepBrightness > 50) {
    Fade->FadeStepBrightness = 5; //-- Init Value
  }

  unsigned long CurMillisFadeStrip = millis();

  //-- Fade Color is disabled as long as a transition is going on
  if (TransData->Finished) {

    //---- Fade Color
    if (CurMillisFadeStrip - Fade->PrevMillisColorFade >= Fade->TimeOutColorFade) {
      Fade->PrevMillisColorFade = CurMillisFadeStrip;

      //-- Check if RGB is supported
      if (HardwareStrip->isRGB) {

        //-- Red
        if (CurData->R < NextData->R) {
          if ((CurData->R + Fade->FadeStepColor) > NextData->R) {
            CurData->R = NextData->R;
          } else {
            CurData->R += Fade->FadeStepColor;
          }
        }
        if (CurData->R > NextData->R) {
          if ((CurData->R - Fade->FadeStepColor) < NextData->R) {
            CurData->R = NextData->R;
          } else {
            CurData->R -= Fade->FadeStepColor;
          }
        }

        //-- Green
        if (CurData->G < NextData->G) {
          if ((CurData->G + Fade->FadeStepColor) > NextData->G) {
            CurData->G = NextData->G;
          } else {
            CurData->G += Fade->FadeStepColor;
          }
        }
        if (CurData->G > NextData->G) {
          if ((CurData->G - Fade->FadeStepColor) < NextData->G) {
            CurData->G = NextData->G;
          } else {
            CurData->G -= Fade->FadeStepColor;
          }
        }

        //-- Blue
        if (CurData->B < NextData->B) {
          if ((CurData->B + Fade->FadeStepColor) > NextData->B) {
            CurData->B = NextData->B;
          } else {
            CurData->B += Fade->FadeStepColor;
          }
        }
        if (CurData->B > NextData->B) {
          if ((CurData->R - Fade->FadeStepColor) < NextData->B) {
            CurData->B = NextData->B;
          } else {
            CurData->B -= Fade->FadeStepColor;
          }
        }

      }

      //-- Check if CW is supported
      if (HardwareStrip->isCW) {

        //-- Cold White
        if (CurData->CW < NextData->CW) {
          if ((CurData->CW + Fade->FadeStepColor) > NextData->CW) {
            CurData->CW = NextData->CW;
          } else {
            CurData->CW += Fade->FadeStepColor;
          }
        }
        if (CurData->CW > NextData->CW) {
          if ((CurData->CW - Fade->FadeStepColor) < NextData->CW) {
            CurData->CW = NextData->CW;
          } else {
            CurData->CW -= Fade->FadeStepColor;
          }
        }

      }

      //-- Check if WW is supported
      if (HardwareStrip->isWW) {

        //-- Warm White
        if (CurData->WW < NextData->WW) {
          if ((CurData->WW + Fade->FadeStepColor) > NextData->WW) {
            CurData->WW = NextData->WW;
          } else {
            CurData->WW += Fade->FadeStepColor;
          }
        }
        if (CurData->WW > NextData->WW) {
          if ((CurData->WW - Fade->FadeStepColor) < NextData->WW) {
            CurData->WW = NextData->WW;
          } else {
            CurData->WW -= Fade->FadeStepColor;
          }
        }

      }

    }

  }

  //---- Fade Brightness
  if (CurMillisFadeStrip - Fade->PrevMillisBrightnessFade >= Fade->TimeOutBrightnessFade) {
    Fade->PrevMillisBrightnessFade = CurMillisFadeStrip;

    //-- Check if RGB or CW or WW is supported
    if (HardwareStrip->isRGB or HardwareStrip->isCW or HardwareStrip->isWW) {

      //-- Brightness
      if (CurData->Brightness < NextData->Brightness) {
        if ((CurData->WW + Fade->FadeStepBrightness) > NextData->Brightness) {
          CurData->Brightness = NextData->Brightness;
        } else {
          CurData->Brightness += Fade->FadeStepBrightness;
        }
      }
      if (CurData->Brightness > NextData->Brightness) {
        if ((CurData->Brightness - Fade->FadeStepBrightness) < NextData->Brightness) {
          CurData->Brightness = NextData->Brightness;
        } else {
          CurData->Brightness -= Fade->FadeStepBrightness;
        }
      }

    }

  }

}


void ShowStrip(HardwareStripConfig * HardwareStrip, StripData * CurData) {

  //-- Check if Strip has a ID and if the Id is valid
  if (HardwareStrip->StripID != 0xff and HardwareStrip->IdValid) {

    //-- Check if RGB is supported
    if (HardwareStrip->isRGB) {
      analogWrite(HardwareStrip->PinRed,   ((CurData->R * 4) * CurData->Brightness) / MAX_BRIGHTNESS_VALUE);
      analogWrite(HardwareStrip->PinGreen, ((CurData->G * 4) * CurData->Brightness) / MAX_BRIGHTNESS_VALUE);
      analogWrite(HardwareStrip->PinBlue,  ((CurData->B * 4) * CurData->Brightness) / MAX_BRIGHTNESS_VALUE);
    }

    //-- Check if CW is supported
    if (HardwareStrip->isCW) {
      analogWrite(HardwareStrip->PinColdWhite, ((CurData->CW * 4) * CurData->Brightness) / MAX_BRIGHTNESS_VALUE);
    }

    //-- Check if WW is supported
    if (HardwareStrip->isWW) {
      analogWrite(HardwareStrip->PinWarmWhite, ((CurData->WW * 4) * CurData->Brightness) / MAX_BRIGHTNESS_VALUE);
    }

  }

}


void ResetEffectData(BasicEffect *BasicEffectData) {

  BasicEffectData->State      = 0;
  BasicEffectData->SubState   = 0;
  BasicEffectData->Counter    = 0;
  BasicEffectData->SubCounter = 0;

}

uint8_t getTimeBasedBrightness() {

  uint8_t temp = 0;

  switch (ParameterHassIO.TimeHour) {

    //-- Night
    case 22:    temp = 224;
      break;
    case 23:    temp = 192;
      break;
    case 0:     temp = 160;
      break;
    case 1:     temp = 128;
      break;
    case 2:     temp = 96;
      break;
    case 3:     temp = 62;
      break;
    case 4:     temp = 96;
      break;
    case 5:     temp = 128;
      break;
    case 6:     temp = 160;
      break;
    case 7:     temp = 192;
      break;
    case 8:     temp = 224;
      break;

    //-- Day max Brightness
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:    temp = 255;
      break;

    default:    temp = 128;
      break;
  }

  return temp;

}
