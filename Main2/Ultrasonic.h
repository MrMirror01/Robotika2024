// robot se na mjestu poravna tako da bude okomit na zid uz pomoć ultrasoničnih senzora
void alignToWall() {
  int distanceL = 0;
  int distanceR = 0;

  int direction = 0;
  int cnt = 0;
  while (true) {
    if (cnt % 25 == 0) {
      distanceL = sonarL.ping_cm();
      distanceR = sonarR.ping_cm();
      direction = (distanceR - distanceL);
    }
    cnt++;

    if (distanceL == 0 || distanceR == 0) {
      stepperL.setSpeed(1000);
      stepperR.setSpeed(1000);
    }
    else if (direction == 0){
      break;
    }
    else if (direction > 0) {
      stepperL.setSpeed(-1000);
      stepperR.setSpeed(1000);
    }
    else {
      stepperL.setSpeed(1000);
      stepperR.setSpeed(-1000);
    }

    stepperL.runSpeed();
    stepperR.runSpeed();
  }
}