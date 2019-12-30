

void leds() {

  //Call Control Functions
  ControlLEDStrip1();
  ControlLEDStrip2();

  //Call Fade Functions
  FadeLEDStrip1();
  FadeLEDStrip2();

  //Call Show Functions
  ShowLEDStrip1();
  ShowLEDStrip2();
}


void ControlLEDStrip1() {

  //-- Next Mode Control
  // Its only possible to swap to another mode when the transition has finished
  if (TransStrip1Finished) {



  }

  //-- Mode Transition Control
  TransitionModeStrip1(NextLEDStrip1Mode);

  //-- Current Mode Control
  if (TransStrip1Finished) {
    //-- Current Mode for the 1 LED Strip
    switch (CurLEDStrip1Mode) {

      case Idle_Mode:
        NextLEDStrip1.Brightness     = 0;
        break;

      case Normal_Mode:
        NextLEDStrip1.Brightness     = ParameterLEDStrip1.Brightness;
        NextLEDStrip1.White          = ParameterLEDStrip1.White;
        NextLEDStrip1.Red            = ParameterLEDStrip1.Red;
        NextLEDStrip1.Green          = ParameterLEDStrip1.Green;
        NextLEDStrip1.Blue           = ParameterLEDStrip1.Blue;
        break;

      case NoWiFi_Mode:

        break;

      case NoMQTT_Mode:

        break;

      case NoHassIO_Mode:

        break;

      case GoodMorning_Mode:

        break;

      case GoodNight_Mode:

        break;

      case Weekend_Mode:

        break;

      case Alarm_Mode:

        break;

      default:
        break;

    }

  }

}

boolean TransitionModeStrip1(LEDModes NextMode) {

  unsigned long CurMillisTransitionSpeedStrip1 = millis();
  boolean temp = false,

  switch (StateTransition) {

    //-- Init
    case 0:
      PrevMillisTransitionSpeedStrip1 = CurMillisTransitionSpeedStrip1;
      StateTransition = 1;
      break;

    //-- Fade Brightness to 0
    case 1:
      if (CurMillisTransitionSpeedStrip1 - PrevMillisTransitionSpeedStrip1 >= 10) {
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

      break;

    //-- Finish and Reset
    case 3:
      StateTransition = 0;
      temp = true;
      break;

  }

  return temp;
}


void ControlLEDStrip2() {

}


void FadeLEDStrip1() {
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


void FadeLEDStrip2() {
  unsigned long CurMillisFadeStrip2 = millis();

  //---- Fade Color LED Strip 2
  if (CurMillisFadeStrip2 - PrevMillisFadeColorStrip2 >= FadeSpeedColorStrip2) {
    PrevMillisFadeColorStrip2 = CurMillisFadeStrip2;
    //-- Red
    if (CurLEDStrip2.Red < NextLEDStrip2.Red) {
      if ((CurLEDStrip2.Red + FadeStepSize) > NextLEDStrip2.Red) {
        CurLEDStrip2.Red = NextLEDStrip2.Red;
      } else {
        CurLEDStrip2.Red += FadeStepSize;
      }
    }
    if (CurLEDStrip2.Red > NextLEDStrip2.Red) {
      if ((CurLEDStrip2.Red - FadeStepSize) < NextLEDStrip2.Red) {
        CurLEDStrip2.Red = NextLEDStrip2.Red;
      } else {
        CurLEDStrip2.Red -= FadeStepSize;
      }
    }

    //-- Green
    if (CurLEDStrip2.Green < NextLEDStrip2.Green) {
      if ((CurLEDStrip2.Green + FadeStepSize) > NextLEDStrip2.Green) {
        CurLEDStrip2.Green = NextLEDStrip2.Green;
      } else {
        CurLEDStrip2.Green += FadeStepSize;
      }
    }
    if (CurLEDStrip2.Green > NextLEDStrip2.Green) {
      if ((CurLEDStrip2.Green - FadeStepSize) < NextLEDStrip2.Green) {
        CurLEDStrip2.Green = NextLEDStrip2.Green;
      } else {
        CurLEDStrip2.Green -= FadeStepSize;
      }
    }

    //-- Blue
    if (CurLEDStrip2.Blue < NextLEDStrip2.Blue) {
      if ((CurLEDStrip2.Blue + FadeStepSize) > NextLEDStrip2.Blue) {
        CurLEDStrip2.Blue = NextLEDStrip2.Blue;
      } else {
        CurLEDStrip2.Blue += FadeStepSize;
      }
    }
    if (CurLEDStrip2.Blue > NextLEDStrip2.Blue) {
      if ((CurLEDStrip2.Blue - FadeStepSize) < NextLEDStrip2.Blue) {
        CurLEDStrip2.Blue = NextLEDStrip2.Blue;
      } else {
        CurLEDStrip2.Blue -= FadeStepSize;
      }
    }

    //-- White
    if (CurLEDStrip2.White < NextLEDStrip2.White) {
      if ((CurLEDStrip2.White + FadeStepSize) > NextLEDStrip2.White) {
        CurLEDStrip2.White = NextLEDStrip2.White;
      } else {
        CurLEDStrip2.White += FadeStepSize;
      }
    }
    if (CurLEDStrip2.White > NextLEDStrip2.White) {
      if ((CurLEDStrip2.White - FadeStepSize) < NextLEDStrip2.White) {
        CurLEDStrip2.White = NextLEDStrip2.White;
      } else {
        CurLEDStrip2.White -= FadeStepSize;
      }
    }
  }

  //---- Fade Brightness LED Strip 2
  if (CurMillisFadeStrip2 - PrevMillisFadeBrightnessStrip2 >= FadeSpeedBrightnessStrip2) {
    PrevMillisFadeBrightnessStrip2 = CurMillisFadeStrip2;
    //-- Brightness
    if (CurLEDStrip2.Brightness < NextLEDStrip2.Brightness) {
      if ((CurLEDStrip2.Brightness + FadeStepSize) > NextLEDStrip2.Brightness) {
        CurLEDStrip2.Brightness = NextLEDStrip2.Brightness;
      } else {
        CurLEDStrip2.Brightness += FadeStepSize;
      }
    }
    if (CurLEDStrip2.Brightness > NextLEDStrip2.Brightness) {
      if ((CurLEDStrip2.Brightness - FadeStepSize) < NextLEDStrip2.Brightness) {
        CurLEDStrip2.Brightness = NextLEDStrip2.Brightness;
      } else {
        CurLEDStrip2.Brightness -= FadeStepSize;
      }
    }
  }
}


void ShowLEDStrip1() {
  if (CONTROLLER_NUM_LED_STRIPS >= 1) {
#ifdef CONTROLLER_RGB or CONTROLLER_RGBW
    analogWrite(PIN_STRIP_1_RED,   ((CurLEDStrip1.Red   * 4) * CurLEDStrip1.Brightness) / 255);
    analogWrite(PIN_STRIP_1_GREEN, ((CurLEDStrip1.Green * 4) * CurLEDStrip1.Brightness) / 255);
    analogWrite(PIN_STRIP_1_BLUE,  ((CurLEDStrip1.Blue  * 4) * CurLEDStrip1.Brightness) / 255);
#endif
#ifdef CONTROLLER_RGBW
    analogWrite(PIN_STRIP_1_WHITE, ((CurLEDStrip1.White * 4) * CurLEDStrip1.Brightness) / 255);
#endif
  }
}


void ShowLEDStrip2() {
  if (CONTROLLER_NUM_LED_STRIPS == 2) {
#ifdef CONTROLLER_RGB or CONTROLLER_RGBW
    analogWrite(PIN_STRIP_2_RED,   ((CurLEDStrip2.Red   * 4) * CurLEDStrip2.Brightness) / 255);
    analogWrite(PIN_STRIP_2_GREEN, ((CurLEDStrip2.Green * 4) * CurLEDStrip2.Brightness) / 255);
    analogWrite(PIN_STRIP_2_BLUE,  ((CurLEDStrip2.Blue  * 4) * CurLEDStrip2.Brightness) / 255);
#endif
#ifdef CONTROLLER_RGBW
    analogWrite(PIN_STRIP_2_WHITE, ((CurLEDStrip2.White * 4) * CurLEDStrip2.Brightness) / 255);
#endif
  }
}
