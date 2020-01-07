

void motion() {

#ifdef CONTROLLER_MOTION_DETECTION

  //-- Motion is deteced when one sensor recognize motion
  if (PIRSensor1.MotionDeteced or PIRSensor2.MotionDeteced) {
    MotionData.MotionDeteced = true;
  }
  if (!PIRSensor1.MotionDeteced and !PIRSensor2.MotionDeteced) {
    MotionData.MotionDeteced = false;
  }

#endif

}


#ifdef CONTROLLER_MOTION_DETECTION
ICACHE_RAM_ATTR void StateMotionDetectedSensor1() {
  //-- Get Motion value
  if (digitalRead(PIN_MOTION_SENSOR_1) == HIGH) {
    PIRSensor1.MotionDeteced = true;
  } else {
    PIRSensor1.MotionDeteced = false;
  }
}

ICACHE_RAM_ATTR void StateMotionDetectedSensor2() {
  //-- Get Motion value
  if (digitalRead(PIN_MOTION_SENSOR_2) == HIGH) {
    PIRSensor2.MotionDeteced = true;
  } else {
    PIRSensor2.MotionDeteced = false;
  }
}
#endif
