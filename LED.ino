

void leds() {

  //-- Call Control Function
  StripControl(&HardwareConfigStrip1, &CurStrip1Data, &NextStrip1Data, &TransitionDataStrip1, &ParameterLEDStrip1);
  StripControl(&HardwareConfigStrip2, &CurStrip2Data, &NextStrip2Data, &TransitionDataStrip2, &ParameterLEDStrip2);

  //-- Call Fade Function
  FadeStrip(&HardwareConfigStrip1, &CurStrip1Data, &NextStrip1Data, &FadeDataStrip1, &TransitionDataStrip1);
  FadeStrip(&HardwareConfigStrip2, &CurStrip2Data, &NextStrip2Data, &FadeDataStrip2, &TransitionDataStrip2);

  //-- Call Show Function
  ShowStrip(&HardwareConfigStrip1, &CurStrip1Data);
  ShowStrip(&HardwareConfigStrip2, &CurStrip2Data);

}


void StripControl(HardwareStripConfig *HardwareStrip, StripData *CurData, StripData *NextData, TransitionData *TransData, StripParameter *Strip) {

  //-- Mode Control
  /*
     Prio from bottom highest to lowest at the top
  */
  if (TransData->Finished) {

    //---- Motion
    // PRIO 3

    //---- Effects
    // PRIO 2
    if (Strip->Power) {
      switch (Strip->Effect) {

        //-- None / Normal mode
        case e_None:
          //-- Set new Mode
          TransData->NextMode = Normal;
          //-- Set Mode Parameter
          //-- Check if RGB is supported
          if (HardwareStrip->isRGB) {
            NextData->R = Strip->Red;
            NextData->G = Strip->Green;
            NextData->B = Strip->Blue;
          }
          //-- Check if CW is supported
          if (HardwareStrip->isCW) {
            NextData->CW = Strip->ColdWhite;
          }
          //-- Check if WW is supported
          if (HardwareStrip->isWW) {
            NextData->CW = Strip->WarmWhite;
          }
          //-- Check if Brightness is used
          if (HardwareStrip->isRGB or HardwareStrip->isCW or HardwareStrip->isWW) {
            NextData->Brightness = Strip->Brightness;
          }
          break;

        //-- Alarm Mode
        case e_Alarm:
          //-- Set new Mode
          TransData->NextMode = Alarm;
          //-- Set Mode Parameter
          NextData->R = MAX_COLOR_VALUE;
          NextData->G = 0;
          NextData->B = 0;
          NextData->CW = 0;
          NextData->WW = 0;
          NextData->Brightness = MAX_BRIGHTNESS_VALUE;
          break;

        //-- WakeUp Mode
        case e_Wakeup:
          //-- Set new Mode
          TransData->NextMode = Wakeup;
          //-- Set Mode Parameter

          break;

        //-- Sleep Mode
        case e_Sleep:
          //-- Set new Mode
          TransData->NextMode = Sleep;
          //-- Set Mode Parameter

          break;

        //-- Weekend Mode
        case e_Weekend:
          //-- Set new Mode
          TransData->NextMode = Weekend;
          //-- Set Mode Parameter

          break;

      }
    } else {
      NextData->Brightness = 0;  //-- Power Off
    }

    //---- Status Messages
    // PRIO 1
    //-- NoHassIO
    if (!NetworkState.HassIO_Connected) {
      //-- Set new Mode
      TransData->NextMode = NoHassIO;
      //-- Set Mode Parameter
      NextData->R = 0;
      NextData->G = MAX_COLOR_VALUE;
      NextData->B = MAX_COLOR_VALUE;
      NextData->CW = 0;
      NextData->WW = 0;
      NextData->Brightness = MAX_BRIGHTNESS_VALUE;
    }

    //-- NoMQTT
    if (!NetworkState.MQTT_Connected) {
      //-- Set new Mode
      TransData->NextMode = NoMQTT;
      //-- Set Mode Parameter
      NextData->R = 0;
      NextData->G = 0;
      NextData->B = MAX_COLOR_VALUE;
      NextData->CW = 0;
      NextData->WW = 0;
      NextData->Brightness = MAX_BRIGHTNESS_VALUE;
    }

    //-- NoWiFi
    if (!NetworkState.Wifi_Connected) {
      //-- Set new Mode
      TransData->NextMode = NoWiFi;
      //-- Set Mode Parameter
      NextData->R = 0;
      NextData->G = MAX_COLOR_VALUE;
      NextData->B = 0;
      NextData->CW = 0;
      NextData->WW = 0;
      NextData->Brightness = MAX_BRIGHTNESS_VALUE;
    }

  }

  //-- Call Mode Transition
  ModeTransition(TransData, CurData, NextData, HardwareStrip);

}


void ModeTransition(TransitionData *TransData, StripData *CurData, StripData *NextData, HardwareStripConfig *HardwareStrip) {

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


void FadeStrip(HardwareStripConfig *HardwareStrip, StripData *CurData, StripData *NextData, FadeData *Fade, TransitionData *TransData) {

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


void ShowStrip(HardwareStripConfig *HardwareStrip, StripData *CurData) {

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
