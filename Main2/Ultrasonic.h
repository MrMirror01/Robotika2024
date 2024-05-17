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

// vozi se ravno do zadane udaljenosti od prepreke
void driveUntilObstacle(int dist) {
  int distanceL = 0;
  int distanceR = 0;

  stepperL.setSpeed(2500);
  stepperR.setSpeed(2500);

  int cnt = 0;
  int stopCnt = 0;
  while (true) {
    if (cnt % 25 == 0) {
      distanceL = sonarL.ping_cm();
      distanceR = sonarR.ping_cm();
    }
    cnt++;

    stepperL.runSpeed();
    stepperR.runSpeed();

    if (distanceL <= dist && distanceR <= dist) {
      stopCnt++;
    }
    else stopCnt = 0;

    if (stopCnt > 100) return;
  }
}

// ispisuje udaljenost na lcd
void printDistance() {
  lcd.clear();
  int distanceL = sonarL.ping_cm();
  int distanceR = sonarR.ping_cm();

  lcd.print((distanceL + distanceR) / 2);
}