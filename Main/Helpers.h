#include "Init.h"

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

void rotateLeft(int steps){
  stepperR.setSpeed(1000);
  stepperL.setSpeed(-1000);
  stepperR.move(steps);
  stepperL.move(-steps);

  while (stepperL.distanceToGo() || stepperR.distanceToGo()) {
    stepperL.run();
    stepperR.run();
  }
}

// skrene lijevo (90 stupnjeva)
void turnLeft() {
  alignForward();

  stepperR.setSpeed(1000);
  stepperL.setSpeed(-1000);
  stepperR.move(1500);
  stepperL.move(-1500);

  while (stepperL.distanceToGo() || stepperR.distanceToGo()) {
    stepperL.run();
    stepperR.run();
  }
}

void rotateRight(int steps){
  stepperR.setSpeed(-1000);
  stepperL.setSpeed(1000);
  stepperR.move(-steps);
  stepperL.move(steps);

  while (stepperL.distanceToGo() || stepperR.distanceToGo()) {
    stepperL.run();
    stepperR.run();
  }
}

// skrene desno (90 stupnjeva)
void turnRight() {
  alignForward();

  stepperR.setSpeed(-1000);
  stepperL.setSpeed(1000);
  stepperR.move(-1500);
  stepperL.move(1500);

  while (stepperL.distanceToGo() || stepperR.distanceToGo()) {
    stepperL.run();
    stepperR.run();
  }
}

// okrene se za 180 stupnjeva
void turnAround() {
  stepperR.setSpeed(-1000);
  stepperL.setSpeed(1000);
  stepperR.move(-3500);
  stepperL.move(3500);

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

int _puck_position_ = 0;

void readColor(){
  lcd.clear();
  color.startMeasurement(); //begin a measurement
  
  //wait till data is available
  bool rdy = false;
  while(!rdy){
    delay(5);
    rdy = color.dataReady();
  }

  color.readRawValues(sensorValues);

  int red = sensorValues[AS726x_RED];
  int green = sensorValues[AS726x_GREEN];
  int blue = sensorValues[AS726x_BLUE];

  if (red >= green && red >= blue) {
    _puck_position_ = 0;
    lcd.print("Crvena");
  }
  else if (blue >= red && blue >= green) {
    _puck_position_ = 1;
    lcd.print("Plava");
  }
  else {
    _puck_position_ = 2;
    lcd.print("Zelena");
  }
}