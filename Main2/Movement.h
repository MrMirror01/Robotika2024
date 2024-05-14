// stepperi se krecu akceleracijski dok ne dostignu zadani cilj
void runSteppers(){
  while (stepperL.distanceToGo() || stepperR.distanceToGo()) {
    stepperL.run();
    stepperR.run();
  }
}

//pomakne se prema naprijed za "steps" koraka (ili natrag ako je "steps" < 0)
void goForward(int steps) {
  // postavi se smjer kretanja
  stepperL.setSpeed((steps < 0 ? -1 : 1));
  stepperR.setSpeed((steps < 0 ? -1 : 1));

  // zada se zeljeni broj koraka
  stepperR.move(steps);
  stepperL.move(steps);

  runSteppers();
}

//pomakne se naprijed tako da straznji kotaci budu tamo gdje su do sad bili senzori za liniju
void alignForward() {
  goForward(ALIGN_STEPS);
}

// rotira se lijevo za zadani broj koraka
void rotateLeft(int steps){
  // postavi se smjer kretanja
  stepperR.setSpeed(1);
  stepperL.setSpeed(-1);

  // zada se zeljeni broj koraka
  stepperR.move(steps);
  stepperL.move(-steps);

  runSteppers();
}

// poravna se tako da skrece oko senzora linije i skrene lijevo (90 stupnjeva)
void turnLeft() {
  alignForward(); // poravnanje
  rotateLeft(RIGHT_ANGLE_STEPS); // rotacija
}

// rotira se desno za zadani broj koraka
void rotateRight(int steps){
  // postavi se smjer kretanja
  stepperR.setSpeed(-1);
  stepperL.setSpeed(1);

  // zada se zeljeni broj koraka
  stepperR.move(-steps);
  stepperL.move(steps);

  runSteppers();
}

// poravna se tako da skrece oko senzora linije i skrene desno (90 stupnjeva)
void turnRight() {
  alignForward(); // poravnanje
  rotateRight(RIGHT_ANGLE_STEPS); // rotacija
}

// okrene se za 180 stupnjeva prema desno
void turnAroundRight() {
  rotateRight(2 * RIGHT_ANGLE_STEPS);
}

// okrene se za 180 stupnjeva prema lijevo
void turnAroundLeft() {
  rotateLeft(2 * RIGHT_ANGLE_STEPS);
}

// vozi se naprijed dok ne dode do puck-a
void goForwardUntilPuck() {
  stepperL.setSpeed(1000);
  stepperR.setSpeed(1000);

  int cnt = 0;
  while (true){
    if (!digitalRead(PUCK_SENSOR_PIN)){
      cnt++;
    }
    else cnt = 0;

    if (cnt >= 3) return;

    stepperL.runSpeed();
    stepperR.runSpeed();
  }
}