

void leds() {

  //---- LED Strip 1

  
  
  switch (StateStrip1) {

    //-- Neutral Mode
    case 0:
      StateStrip1 = 1;
      break;

    //-- Basic Mode
    case 1:
      if (FirstStrip.Power) {
        LEDStrip1.Brightness  = FirstStrip.Brightness;
        LEDStrip1.White       = FirstStrip.White;
        LEDStrip1.Red         = FirstStrip.Red;
        LEDStrip1.Green       = FirstStrip.Green;
        LEDStrip1.Blue        = FirstStrip.Blue;
      } else {
        LEDStrip1.Brightness  = 0;
      }
      break;

    //-- Motion Mode
    case 2:

      break;

    //-- Alarm Mode
    case 3:

      break;

    //-- Wakeup Mode
    case 4:

      break;

    //-- Sleep Mode
    case 5:

      break;

    //-- Weekend Mode
    case 6:

      break;

    //-- Transition Mode
    case 10:

      break;

  }


  //---- LED Strip 2
  switch (StateStrip2) {

    //-- Neutral Mode
    case 0:
      StateStrip2 = 1;
      break;

    //-- Basic Mode
    case 1:
      if (SecondStrip.Power) {
        LEDStrip2.Brightness  = SecondStrip.Brightness;
        LEDStrip2.White       = SecondStrip.White;
        LEDStrip2.Red         = SecondStrip.Red;
        LEDStrip2.Green       = SecondStrip.Green;
        LEDStrip2.Blue        = SecondStrip.Blue;
      } else {
        LEDStrip2.Brightness  = 0;
      }
      break;

    //-- Motion Mode
    case 2:

      break;

    //-- Alarm Mode
    case 3:

      break;

    //-- Wakeup Mode
    case 4:

      break;

    //-- Sleep Mode
    case 5:

      break;

    //-- Weekend Mode
    case 6:

      break;

    //-- Transition Mode
    case 10:

      break;

  }

  //Call Show Functions
  ShowLEDStrip1();
  ShowLEDStrip2();
}



void ShowLEDStrip1() {
  analogWrite(PIN_STRIP_1_RED,   ((LEDStrip1.Red   * 4) * LEDStrip1.Brightness) / 255);
  analogWrite(PIN_STRIP_1_GREEN, ((LEDStrip1.Green * 4) * LEDStrip1.Brightness) / 255);
  analogWrite(PIN_STRIP_1_BLUE,  ((LEDStrip1.Blue  * 4) * LEDStrip1.Brightness) / 255);
  analogWrite(PIN_STRIP_1_WHITE, ((LEDStrip1.White * 4) * LEDStrip1.Brightness) / 255);
}


void ShowLEDStrip2() {
  analogWrite(PIN_STRIP_2_RED,   ((LEDStrip2.Red   * 4) * LEDStrip2.Brightness) / 255);
  analogWrite(PIN_STRIP_2_GREEN, ((LEDStrip2.Green * 4) * LEDStrip2.Brightness) / 255);
  analogWrite(PIN_STRIP_2_BLUE,  ((LEDStrip2.Blue  * 4) * LEDStrip2.Brightness) / 255);
  analogWrite(PIN_STRIP_2_WHITE, ((LEDStrip2.White * 4) * LEDStrip2.Brightness) / 255);
}
