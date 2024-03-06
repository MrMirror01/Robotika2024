//https://github.com/danionescu0/arduino/blob/master/projects/line_follower/line_follower.ino
#include "Init.h"
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

  putGrabberUp();
  resetShoot();
  delay(1000);
  //goForward(-1000);
  //shoot();
  //pickUpPuck();
}

bool line[8];
int numSensors;
float direction = 0;
float lastDirection = 0;

int fullLine = 0;
int halfLine = 0;

/*
_stage_
0 -> prati crtu do oznake, skrene lijevo
1 -> ide ravno do crte, skrene desno
2 -> prati crtu do oznake
3 -> vozi ravno do puck-a, primi ga i podigne
4 -> okrece se, vozi ravno do zida, skrene lijevo, vozi do zida, skrene lijevo, vozi do zida, skrene desno
5 -> vozi ravno do oznake
6 -> prati crtu, broji oznake, do X-te
7 -> ako je X=1 ide ravno, inace skrene lijevo i ide ravno, ostavi puck, odmakne se, skrene lijevo
8 -> vozi do crte
9 -> skrene desno, vozi unatrag do pucka
10 -> ispucava
*/
int _stage_ = 0;

int _puck_position_ = 0;
int currentLine = 0;

// ocita senzore za liniju te u globalne varijable zapise
// koliko senzora vidi crnu boju te u kojem smjeru bi se robot trebao kretati
void scanLine() {
  direction = 0;
  numSensors = 0;
  for (int i = 0; i < 8; i++) {
    line[i] = digitalRead(LINE_FOLLOW_PIN - 2 * i);
    if (line[i]) numSensors++;
    if (i < 4) {
      direction -= line[i] * (4 - i);
    } else {
      direction += line[i] * (i - 3);
    }
    //Serial.print(line[i]);
  }
  //Serial.println("");
  direction /= 10;
}

//pozicionira se u odnosu na puck te ga podize
void pickUpPuck() {
  openGrabber();
  delay(1000);
  driveUntilWall(10);
  alignToWall();
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

    if (distanceL == 0 || distanceR == 0) {
      direction = 0;
    } else {
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

    if ((distanceL < dist || distanceR < dist) && distanceL != 0 && distanceR != 0) {
      endCheck++;
      if (endCheck == 5) return;
    } else endCheck = 0;
  }
}

// voznja bez pracenja linije
void stage4() {
  turnAround();
  driveUntilWall(10);
  turnLeft();
  driveUntilWall(10);
  turnLeft();
  driveUntilWall(10);
  turnRight();
}

//ostavljanje pucka na poziciji
void stage7() {
  goForward(1500);
  letGoOfPuck();
  goForward(-1500);
}

void loop() {
  if (_stage_ == 10) return;

  scanLine();

  //ako senzor ne vidi liniju nastavlja skretati u istom smjeru
  if (numSensors == 0) {
    direction = lastDirection;
  }
  lastDirection = direction;

  // detekcija pune oznake na crti -> svih osam senzora vidi crtu
  if (numSensors == 8) {
    fullLine++;
    if (fullLine == 30) {
      fullLine = 0;
      if (_stage_ == 0) {
        turnLeft();

        _stage_++;
      } else if (_stage_ == 1) {
        turnRight();

        _stage_++;
      } else if (_stage_ == 2) {
        pickUpPuck();
        _stage_++;

        stage4();
        _stage_++;
      } else if (_stage_ == 5) {
        _stage_++;
      } else if (_stage_ == 8) {
        turnRight();
        _stage_++;
      }
      if (_stage_ == 9) {
        shoot();
        _stage_++;
      }
    }
  } else {
    fullLine = 0;
  }

  //detekcija polovične linije i brojanje (prilikom ostavljanja pucka)
  if (_stage_ == 6 && numSensors > 4) {
    halfLine++;

    if (halfLine == 5) {
      if (currentLine == 0 && direction > 0) {
        if (currentLine == _puck_position_) {
          _stage_++;
          stage7();
          turnLeft();
        } else {
          turnRight();
        }
      } else if (currentLine != 0 && direction < 0) {
        if (currentLine == _puck_position_) {
          _stage_++;
          turnLeft();
          stage7();
          turnLeft();
          _stage_++;
        } else alignForward();
      }
    }

    halfLine = 0;
  } else halfLine = 0;

  // praćenje linije
  if (_stage_ <= 2 || _stage_ == 5 || _stage_ == 6 || _stage_ == 8) {
    stepperR.setSpeed(1500 - (direction * 1500));
    stepperL.setSpeed(1500 + (direction * 1500));

    stepperL.runSpeed();
    stepperR.runSpeed();
  } else if (_stage_ == 9) {
    stepperR.setSpeed(-1500 - (direction * 1500));
    stepperL.setSpeed(-1500 + (direction * 1500));

    stepperL.runSpeed();
    stepperR.runSpeed();
  }
}