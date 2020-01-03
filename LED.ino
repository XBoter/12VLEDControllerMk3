

void leds() {

  //-- Call Control Functions
  Control(HardwareConfigStrip1, CurStrip1Data);
  Control(HardwareConfigStrip2, CurStrip2Data);

  //-- Call Fade Functions
  FadeStrip(HardwareConfigStrip1, CurStrip1Data);
  FadeStrip(HardwareConfigStrip2, CurStrip2Data);

  //-- Call Show Functions
  ShowStrip(HardwareConfigStrip1, CurStrip1Data);
  ShowStrip(HardwareConfigStrip2, CurStrip2Data);
}


void ControlStrip(HardwareStripConfig HardwareStrip, StripData Data) {

  //-- Next Mode Control
  if (TransStrip1Finished) {

    switch (ParameterLEDStrip1.Effect) {

      case None:
        NextLEDStrip1Mode = Normal_Mode;
        break;

      case Alarm:
        NextLEDStrip1Mode = NoWiFi_Mode;
        break;

      case Wakeup:
        NextLEDStrip1Mode = NoMQTT_Mode;
        break;

      case Sleep:
        NextLEDStrip1Mode = NoHassIO_Mode;
        break;

      case Weekend:
        NextLEDStrip1Mode = NoWiFi_Mode;
        break;

    }

    //-- Current Mode for the 1 LED Strip
    switch (NextLEDStrip1Mode) {

      case Idle_Mode:         //-- No Color display
        NextLEDStrip1.Brightness     = 0;
        break;

      case Normal_Mode:       //-- Solid Color display
        NextLEDStrip1.Brightness     = ParameterLEDStrip1.Brightness;
        NextLEDStrip1.White          = ParameterLEDStrip1.White;
        NextLEDStrip1.Red            = ParameterLEDStrip1.Red;
        NextLEDStrip1.Green          = ParameterLEDStrip1.Green;
        NextLEDStrip1.Blue           = ParameterLEDStrip1.Blue;
        break;

      case NoWiFi_Mode:       //-- 3 Green flashes
        NextLEDStrip1.Brightness    = 255;
        NextLEDStrip1.White         = 0;
        NextLEDStrip1.Red           = 0;
        NextLEDStrip1.Green         = 255;
        NextLEDStrip1.Blue          = 0;
        break;

      case NoMQTT_Mode:       //-- 3 Blue flashes
        NextLEDStrip1.Brightness    = 255;
        NextLEDStrip1.White         = 0;
        NextLEDStrip1.Red           = 0;
        NextLEDStrip1.Green         = 0;
        NextLEDStrip1.Blue          = 255;
        break;

      case NoHassIO_Mode:     //-- 3 Cyan flashes
        NextLEDStrip1.Brightness    = 255;
        NextLEDStrip1.White         = 0;
        NextLEDStrip1.Red           = 255;
        NextLEDStrip1.Green         = 0;
        NextLEDStrip1.Blue          = 0;
        break;

      case GoodMorning_Mode:  //-- Brightness fading in

        break;

      case GoodNight_Mode:    //-- Brightness fading off

        break;

      case Weekend_Mode:      //-- Color cyle display

        break;

      case Alarm_Mode:        //-- Red flashing

        break;

      default:                //-- Solid Red Display
        NextLEDStrip1.Brightness    = 128;
        NextLEDStrip1.White         = 0;
        NextLEDStrip1.Red           = 255;
        NextLEDStrip1.Green         = 0;
        NextLEDStrip1.Blue          = 0;
        break;

    }

  }

  //-- Call Transition
  TransStrip1Finished = TransitionModeStrip1(NextLEDStrip1Mode);

}

boolean TransitionModeStrip1(LEDModes NextMode) {

  unsigned long CurMillisTransitionSpeedStrip1 = millis();
  boolean temp = false;

  switch (StateTransition) {

    //-- Init / Idle
    case 0:
      PrevMillisTransitionSpeedStrip1 = CurMillisTransitionSpeedStrip1;
      temp = true;
      //Check if next mode is diffrent
      if (NextMode != CurLEDStrip1Mode) {
        StateTransition = 1;
        temp = false;
      }
      break;

    //-- Fade Brightness to 0
    case 1:
      if (CurMillisTransitionSpeedStrip1 - PrevMillisTransitionSpeedStrip1 >= 5) {
        PrevMillisTransitionSpeedStrip1 = CurMillisTransitionSpeedStrip1;
        if ((CurLEDStrip1.Brightness - 5) <= 0) {
          CurLEDStrip1.Brightness = 0;
          StateTransition = 2;
        } else {
          CurLEDStrip1.Brightness -= 5;
        }
      }
      break;

    //-- Set Color of new mode
    case 2:
      //-- RGB
      CurLEDStrip1.Red    = NextLEDStrip1.Red;
      CurLEDStrip1.Green  = NextLEDStrip1.Green;
      CurLEDStrip1.Blue   = NextLEDStrip1.Blue;
      //-- RGBW
      CurLEDStrip1.White  = NextLEDStrip1.White;
      //-- Update Time
      PrevMillisTransitionSpeedStrip1 = CurMillisTransitionSpeedStrip1;
      StateTransition = 3;
      break;

    //-- Fade Brightness to Value from next Mode
    case 3:
      if (CurMillisTransitionSpeedStrip1 - PrevMillisTransitionSpeedStrip1 >= 5) {
        PrevMillisTransitionSpeedStrip1 = CurMillisTransitionSpeedStrip1;
        if ((CurLEDStrip1.Brightness + 5) >= NextLEDStrip1.Brightness) {
          CurLEDStrip1.Brightness = 0;
          StateTransition = 4;
        } else {
          CurLEDStrip1.Brightness += 5;
        }
      }
      break;

    //-- Finish and Reset
    case 4:
      CurLEDStrip1Mode = NextMode;
      StateTransition = 0;
      temp = true;
      break;

  }

  return temp;
}




void FadeStrip(HardwareStripConfig HardwareStrip, StripData Data) {

  unsigned long CurMillisFadeStrip1 = millis();

  //---- Fade Color LED Strip 1
  if (CurMillisFadeStrip1 - PrevMillisFadeColorStrip1 >= FadeSpeedColorStrip1) {
    PrevMillisFadeColorStrip1 = CurMillisFadeStrip1;
    //-- Red
    if (CurLEDStrip1.Red < NextLEDStrip1.Red) {
      if ((CurLEDStrip1.Red + FadeStepSize) > NextLEDStrip1.Red) {
        CurLEDStrip1.Red = NextLEDStrip1.Red;
      } else {
        CurLEDStrip1.Red += FadeStepSize;
      }
    }
    if (CurLEDStrip1.Red > NextLEDStrip1.Red) {
      if ((CurLEDStrip1.Red - FadeStepSize) < NextLEDStrip1.Red) {
        CurLEDStrip1.Red = NextLEDStrip1.Red;
      } else {
        CurLEDStrip1.Red -= FadeStepSize;
      }
    }

    //-- Green
    if (CurLEDStrip1.Green < NextLEDStrip1.Green) {
      if ((CurLEDStrip1.Green + FadeStepSize) > NextLEDStrip1.Green) {
        CurLEDStrip1.Green = NextLEDStrip1.Green;
      } else {
        CurLEDStrip1.Green += FadeStepSize;
      }
    }
    if (CurLEDStrip1.Green > NextLEDStrip1.Green) {
      if ((CurLEDStrip1.Green - FadeStepSize) < NextLEDStrip1.Green) {
        CurLEDStrip1.Green = NextLEDStrip1.Green;
      } else {
        CurLEDStrip1.Green -= FadeStepSize;
      }
    }

    //-- Blue
    if (CurLEDStrip1.Blue < NextLEDStrip1.Blue) {
      if ((CurLEDStrip1.Blue + FadeStepSize) > NextLEDStrip1.Blue) {
        CurLEDStrip1.Blue = NextLEDStrip1.Blue;
      } else {
        CurLEDStrip1.Blue += FadeStepSize;
      }
    }
    if (CurLEDStrip1.Blue > NextLEDStrip1.Blue) {
      if ((CurLEDStrip1.Blue - FadeStepSize) < NextLEDStrip1.Blue) {
        CurLEDStrip1.Blue = NextLEDStrip1.Blue;
      } else {
        CurLEDStrip1.Blue -= FadeStepSize;
      }
    }

    //-- White
    if (CurLEDStrip1.White < NextLEDStrip1.White) {
      if ((CurLEDStrip1.White + FadeStepSize) > NextLEDStrip1.White) {
        CurLEDStrip1.White = NextLEDStrip1.White;
      } else {
        CurLEDStrip1.White += FadeStepSize;
      }
    }
    if (CurLEDStrip1.White > NextLEDStrip1.White) {
      if ((CurLEDStrip1.White - FadeStepSize) < NextLEDStrip1.White) {
        CurLEDStrip1.White = NextLEDStrip1.White;
      } else {
        CurLEDStrip1.White -= FadeStepSize;
      }
    }
  }

  //---- Fade Brightness LED Strip 1
  if (CurMillisFadeStrip1 - PrevMillisFadeBrightnessStrip1 >= FadeSpeedBrightnessStrip1) {
    PrevMillisFadeBrightnessStrip1 = CurMillisFadeStrip1;
    //-- Brightness
    if (CurLEDStrip1.Brightness < NextLEDStrip1.Brightness) {
      if ((CurLEDStrip1.Brightness + FadeStepSize) > NextLEDStrip1.Brightness) {
        CurLEDStrip1.Brightness = NextLEDStrip1.Brightness;
      } else {
        CurLEDStrip1.Brightness += FadeStepSize;
      }
    }
    if (CurLEDStrip1.Brightness > NextLEDStrip1.Brightness) {
      if ((CurLEDStrip1.Brightness - FadeStepSize) < NextLEDStrip1.Brightness) {
        CurLEDStrip1.Brightness = NextLEDStrip1.Brightness;
      } else {
        CurLEDStrip1.Brightness -= FadeStepSize;
      }
    }
  }

}


void ShowStrip(HardwareStripConfig HardwareStrip, StripData Data) {

  //-- Check if Strip has a ID and if the Id is valid
  if (HardwareStrip.StripID != 0xff and HardwareStrip.IdValid) {

    //-- Check if RGB is supported
    if (HardwareStrip.isRGB) {
      analogWrite(HardwareStrip.PinRed,   ((Data.R * 4) * Data.Brightness) / 255);
      analogWrite(HardwareStrip.PinGreen, ((Data.G * 4) * Data.Brightness) / 255);
      analogWrite(HardwareStrip.PinBlue,  ((Data.B * 4) * Data.Brightness) / 255);
    }

    //-- Check if CW is supported
    if (HardwareStrip.isCW) {
      analogWrite(HardwareStrip.PinColdWhite, ((Data.CW * 4) * Data.Brightness) / 255);
    }

    //-- Check if WW is supported
    if (HardwareStrip.isWW) {
      analogWrite(HardwareStrip.PinWarmWhite, ((Data.WW * 4) * Data.Brightness) / 255);
    }

  }

}
