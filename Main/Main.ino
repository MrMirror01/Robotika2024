//https://github.com/danionescu0/arduino/blob/master/projects/line_follower/line_follower.ino
#include "Helpers.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);


  for (int i = 0; i < 8; i++) {
    pinMode(LINE_FOLLOW_PIN + i, INPUT);
  }
  pinMode(LINE_FOLLOW_ENABLE_PIN, OUTPUT);
  digitalWrite(LINE_FOLLOW_ENABLE_PIN, HIGH);

  triggerServo.attach(TRIGGER_SERVO_PIN);
  triggerServo.write(180);

  grabberServoUD.attach(GRABBER_SERVO_UD_PIN);
  grabberServoOpenClose.attach(GRABBER_SERVO_OPENCLOSE_PIN);

  //FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  stepperL.setMaxSpeed(5000);
  stepperR.setMaxSpeed(5000);
  stepperL.setSpeed(2500);
  stepperR.setSpeed(2500);
  stepperL.setAcceleration(2500);
  stepperR.setAcceleration(2500);

  lcd.init();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.print("Hello, world!");

  color.begin();

  putGrabberUp();
  resetShoot();
  delay(1000);

  goForward(4000);
  turnRight();
  goForward(1300);
  alignForward();
  rotateRight(500);
  readColor();
  rotateRight(1000);
  goForward(-750);
}

bool line[8];
int numSensors;
float direction = 0;
float lastDirection = 0;

int fullLine = 0;
int halfLine = 0;

void Modul3(){
  int lastTime = 0;

  while (true){
    if (millis() - lastTime > 2000){
      readColor();
      delay(1000);
      lastTime = millis();
    }

    int distL = sonarL.ping_cm();
    lcd.clear();
    lcd.print(distL);
    delay(100);
  }
}

/*
_stage_
0 -> prati crtu do (drugoga) halflinea, skrene desno, do oznake
1 -> cita boju
2 -> pick up puck
3 -> vozi S curve do udaljenosti X, skrene desno
4 -> ostavi puck, skrene lijevo
5 -> do oznake, okrene se
6 -> puca
*/
int _stage_ = 0;

int currentLine = 0;

// ocita senzore za liniju te u globalne varijable zapise
// koliko senzora vidi crnu boju te u kojem smjeru bi se robot trebao kretati
void scanLine() {
  direction = 0;
  numSensors = 0;
  for (int i = 0; i < 8; i++) {
    line[i] = digitalRead(LINE_FOLLOW_PIN - 2 * i);
    if (line[i]) numSensors++;
    if (i > 1 && i < 4) {
      direction += line[i] * (4 - i);
    } else if (i < 6) {
      direction -= line[i] * (i - 3);
    }
    //Serial.print(line[i]);
  }
  //Serial.println("");
  direction /= 7;
}

//pozicionira se u odnosu na puck te ga podize
void pickUpPuck() {
  goForward(1500);
  goForward(-500);
  putGrabberDown();
  delay(1000);
  closeGrabber();
  delay(2000);
  putGrabberUpSlow();
}

//ispusta puck
void letGoOfPuck() {
  putGrabberDownSlow();
  openGrabber();
  delay(2000);
  putGrabberUp();
}

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
    } else if (direction == 0) break;
    else if (direction > 0) {
      stepperL.setSpeed(-1000);
      stepperR.setSpeed(1000);
    } else {
      stepperL.setSpeed(1000);
      stepperR.setSpeed(-1000);
    }

    stepperL.runSpeed();
    stepperR.runSpeed();
  }

  stepperR.setSpeed(1000);
  stepperL.setSpeed(1000);
}

// robot se priblizava zidu dok ne dode do udaljenosti "dist"
// pokusava ostati okomit na zid
void driveUntilWall(int dist) {
  int distanceL = 0;
  int distanceR = 0;

  float direction = 0;
  int cnt = 0;
  int endCheck = 0;

  while (true) {
    if (cnt % 50 == 0) {
      distanceL = sonarL.ping_cm();
      distanceR = sonarR.ping_cm();
    }
    cnt++;

    if (distanceL == 0 || distanceR == 0) { //ako uopce ne detektira zid sa jednim od senzora nastavi ravno
      direction = 0;
    } else {
      //poravnava se s zidom
      direction = (distanceR - distanceL) * 10;
      direction = max((float)-100, min((float)100, direction));
      direction /= 100;
    }

    if (distanceL < dist && distanceL != 0) {
      stepperL.setSpeed(0);
      stepperR.setSpeed(1500);
    } else if (distanceR < dist && distanceR != 0) {
      stepperL.setSpeed(1500);
      stepperR.setSpeed(0);
    } else {
      stepperR.setSpeed(1500 + (direction * 1500));
      stepperL.setSpeed(1500 - (direction * 1500));
    }

    stepperL.runSpeed();
    stepperR.runSpeed();

    //provjera je li robot postigao zeljenu udaljenost
    if ((distanceL < dist || distanceR < dist) && distanceL != 0 && distanceR != 0) {
      endCheck++;
      if (endCheck == 200) return;
    } else endCheck = 0;
  }
}

// ocitanje boje sa kartice
void stage1(){
  //rotateLeft(1000);
  //readColor();
  //rotateRight(1000);
}

// voznja bez pracenja linije
void stage3() {
  goForward(3500);
  alignToWall();
  //driveUntilWall(10);
  rotateLeft(1500);
  //driveUntilWall(10);
  goForward(4000);
  alignToWall();
  rotateLeft(1500);
  //driveUntilWall(10);
  goForward(4000);
  alignToWall();
  rotateRight(1500);

  goForward(5000);
  rotateRight(1500);
  goForward(3300);

  if (_puck_position_ == 0){
    rotateRight(350);
    goForward(200);
    letGoOfPuck();
    goForward(-200);
    rotateLeft(350);
  }
  else if (_puck_position_ == 1){
  }
  else {
    rotateLeft(350);
    goForward(200);
    letGoOfPuck();
    goForward(-200);
    rotateRight(350);
  }
}

//ostavljanje pucka na poziciji
void stage7() {
  goForward(1500);
  letGoOfPuck();
  goForward(-1500);
}

void loop() {
  if (_stage_ > 6) return;

  scanLine();

  //ako senzor ne vidi liniju nastavlja skretati u istom smjeru
  if (numSensors == 0) {
    direction = lastDirection;
  }
  lastDirection = direction;

  // detekcija pune oznake na crti -> svih osam senzora vidi crtu
  if (numSensors == 8) {
    fullLine++;
    if (fullLine == 50) {
      fullLine = 0;
      if (_stage_ == 0) {
        _stage_++;
        stage1();
        _stage_++;
        pickUpPuck();
        _stage_++;
        stage3();
        _stage_++;
        putGrabberUp();
        goForward(-4000);
      }
      else if (_stage_ == 4){
        turnLeft();
        _stage_++;
      }
       else if (_stage_ == 5) {
        goForward(1500);
        goForward(-500);
        turnAround();
        goForward(-2000);
        _stage_++;
        shoot();
        _stage_++;
      }
    }
  } else {
    fullLine = 0;
  }

  //detekcija polovične linije i brojanje (prilikom ostavljanja pucka)
  /*if (_stage_ == 0 && numSensors > 4) {
    halfLine++;

    if (halfLine == 30){
      if (_stage_ == 0){
        turnRight();
      }
    }

    halfLine = 0;
  } else halfLine = 0;*/

  // praćenje linije
  if (_stage_ == 0 || _stage_ == 5) {
    stepperR.setSpeed(1500 - (direction * 1500));
    stepperL.setSpeed(1500 + (direction * 1500));

    stepperL.runSpeed();
    stepperR.runSpeed();
  }
  else if (_stage_ == 4){
    stepperR.setSpeed(1500);
    stepperL.setSpeed(1500);

    stepperL.runSpeed();
    stepperR.runSpeed();
  }
}