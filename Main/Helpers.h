//pomakne se prema naprijed za "steps" koraka (ili natrag ako je "steps" < 0)
void goForward(int steps) {
  stepperL.setSpeed((steps < 0 ? -1000 : 1000));
  stepperR.setSpeed((steps < 0 ? -1000 : 1000));

  stepperR.move(steps);
  stepperL.move(steps);

  while (stepperL.distanceToGo() || stepperR.distanceToGo()) {
    stepperL.run();
    stepperR.run();
  }

  stepperL.setSpeed(1000);
  stepperR.setSpeed(1000);
}

//pomakne se naprijed tako da straznji kotaci budu tamo gdje su do sad bili senzori za liniju
void alignForward() {
  goForward(1500);
}

// skrene lijevo (90 stupnjeva)
void turnLeft() {
  alignForward();

  stepperR.move(1500);
  stepperL.move(-1500);

  while (stepperL.distanceToGo() || stepperR.distanceToGo()) {
    stepperL.run();
    stepperR.run();
  }
}

// skrene desno (90 stupnjeva)
void turnRight() {
  alignForward();

  stepperR.move(-1500);
  stepperL.move(1500);

  while (stepperL.distanceToGo() || stepperR.distanceToGo()) {
    stepperL.run();
    stepperR.run();
  }
}

// okrene se za 180 stupnjeva
void turnAround() {
  stepperR.move(3000);
  stepperL.move(-3000);

  while (stepperL.distanceToGo() || stepperR.distanceToGo()) {
    stepperL.run();
    stepperR.run();
  }
}

void openGrabber() {
  grabberServoOpenClose.write(140);
}
void closeGrabber() {
  grabberServoOpenClose.write(0);
}
void putGrabberUp() {
  grabberServoUD.write(60);
}
void putGrabberUpSlow() {
  for (int i = 0; i < 60; i++) {
    grabberServoUD.write(i);
    delay(10);
  }
}
void putGrabberDown() {
  grabberServoUD.write(0);
}
void putGrabberDownSlow() {
  for (int i = 60; i >= 0; i--) {
    grabberServoUD.write(i);
    delay(10);
  }
}

//ispucavanje
void shoot() {
  triggerServo.write(110);
}
// postavlja servo za ispucavanje u pocetni polozaj
void resetShoot() {
  triggerServo.write(140);
}