//https://github.com/danionescu0/arduino/blob/master/projects/line_follower/line_follower.ino

//#include <FastLED.h>
#include <AccelStepper.h>
#include <NewPing.h>
#include <Servo.h>

//#define NUM_LEDS 24
//#define LED_PIN 12
#define LINE_FOLLOW_PIN 50
#define LINE_FOLLOW_ENABLE_PIN 52
//CRGB leds[NUM_LEDS];

#define STEP_DIR_PIN_R 8
#define STEP_PIN_R 9
#define STEP_DIR_PIN_L 7
#define STEP_PIN_L 6
#define MOTOR_INTERFACE_TYPE 1
AccelStepper stepperL = AccelStepper(MOTOR_INTERFACE_TYPE, STEP_PIN_L, STEP_DIR_PIN_L);
AccelStepper stepperR = AccelStepper(MOTOR_INTERFACE_TYPE, STEP_PIN_R, STEP_DIR_PIN_R);

#define SONAR_TRIGGER_PIN_L 12
#define SONAR_ECHO_PIN_L 11
#define SONAR_TRIGGER_PIN_R 9
#define SONAR_ECHO_PIN_R 10
#define MAX_DISTANCE 400
NewPing sonarL(SONAR_TRIGGER_PIN_L, SONAR_ECHO_PIN_L, MAX_DISTANCE);
NewPing sonarR(SONAR_TRIGGER_PIN_R, SONAR_ECHO_PIN_R, MAX_DISTANCE);

#define TRIGGER_SERVO_PIN 9
Servo triggerServo;
#define GRABBER_SERVO_UD_PIN 3
#define GRABBER_SERVO_OPENCLOSE_PIN 4
Servo grabberServoUD;
Servo grabberServoOpenClose;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  for (int i = 0; i < 8; i++) {
    pinMode(LINE_FOLLOW_PIN + i, INPUT);
  }
  pinMode(LINE_FOLLOW_ENABLE_PIN, OUTPUT);
  digitalWrite(LINE_FOLLOW_ENABLE_PIN, HIGH);

  triggerServo.attach(TRIGGER_SERVO_PIN);
  triggerServo.write(0);

  grabberServoUD.attach(GRABBER_SERVO_UD_PIN);
  grabberServoOpenClose.attach(GRABBER_SERVO_OPENCLOSE_PIN);

  //FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  stepperL.setMaxSpeed(2500);
  stepperR.setMaxSpeed(2500);
  stepperL.setSpeed(2500);
  stepperR.setSpeed(2500);
  stepperL.setAcceleration(1000);
  stepperR.setAcceleration(1000);

  closeGrabber();
  putGrabberUp();
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

void scanLine() {
  direction = 0;
  numSensors = 0;
  for (int i = 0; i < 8; i++) {
    line[i] = digitalRead(LINE_FOLLOW_PIN - 2 * i);
    if (line[i]) numSensors++;
    if (i < 4) {
      direction -= line[i];
    } else {
      direction += line[i];
    }
    //Serial.print(line[i]);
  }
  //Serial.println("");
  direction /= 4;
}

void alignForward() {
  stepperR.move(1500);
  stepperL.move(1500);

  while (stepperL.distanceToGo() || stepperR.distanceToGo()) {
    stepperL.run();
    stepperR.run();
  }
}

void turnLeft() {
  alignForward();

  stepperR.move(1500);
  stepperL.move(-1500);

  while (stepperL.distanceToGo() || stepperR.distanceToGo()) {
    stepperL.run();
    stepperR.run();
  }
}

void turnRight() {
  alignForward();

  stepperR.move(-1500);
  stepperL.move(1500);

  while (stepperL.distanceToGo() || stepperR.distanceToGo()) {
    stepperL.run();
    stepperR.run();
  }
}

void turnAround() {
  stepperR.move(3000);
  stepperL.move(-3000);

  while (stepperL.distanceToGo() || stepperR.distanceToGo()) {
    stepperL.run();
    stepperR.run();
  }
}

void openGrabber(){
  grabberServoOpenClose.write(180);
}
void closeGrabber(){
  grabberServoOpenClose.write(0);
}
void putGrabberUp(){
  grabberServoUD.write(90);
}
void putGrabberDown(){
  grabberServoUD.write(0);
}

void pickUpPuck() {
  openGrabber();
  driveUntilWall(10);
  putGrabberDown();
  closeGrabber();
}
void letGoOfPuck() {
  openGrabber();
  putGrabberUp();
}

void shoot() {
  triggerServo.write(40);
}

void driveUntilWall(int dist) {
  stepperL.setSpeed(1000);
  stepperR.setSpeed(1000);

  int distanceL = 0;
  int distanceR = 0;

  float direction = 0;
  int cnt = 0;
  int endCheck = 0;

  do {
    if (cnt % 75 == 0) {
      distanceL = sonarL.ping_cm();
      distanceR = sonarR.ping_cm();
    }
    cnt++;

    if (distanceL == 0 || distanceR == 0) {
      direction = 0;
    } else {
      direction = distanceR - distanceL;
      direction = max((float)-10, min((float)10, direction));
      direction /= 10;
    }

    if (distanceL < dist && distanceL != 0) {
      stepperL.setSpeed(0);
      stepperR.setSpeed(1000);
    } else if (distanceR < dist && distanceR != 0) {
      stepperL.setSpeed(1000);
      stepperR.setSpeed(0);
    } else {
      stepperR.setSpeed(1000 + (direction * 1000));
      stepperL.setSpeed(1000 - (direction * 1000));
    }

    stepperL.runSpeed();
    stepperR.runSpeed();

    if ((distanceL < dist || distanceR < dist) && distanceL != 0 && distanceR != 0) {
      endCheck++;
      if (endCheck == 3) return;
    }
    else endCheck = 0;
  } while (true);
}

void stage4() {
  turnAround();
  driveUntilWall(10);
  turnLeft();
  driveUntilWall(10);
  turnLeft();
  driveUntilWall(10);
  turnRight();
}

void stage7() {
  stepperR.move(1500);
  stepperL.move(1500);

  while (stepperL.distanceToGo() || stepperR.distanceToGo()) {
    stepperL.run();
    stepperR.run();
  }

  letGoOfPuck();

  stepperR.move(-1500);
  stepperL.move(-1500);

  while (stepperL.distanceToGo() || stepperR.distanceToGo()) {
    stepperL.run();
    stepperR.run();
  }
}

void loop() {
  if (_stage_ == 10) return;

  scanLine();

  if (numSensors == 0) {
    direction = lastDirection;
  }
  if (numSensors == 8) { // ------------------7 senzora----------------
    fullLine++;
    if (fullLine == 5) {
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

  if (numSensors > 4) {
    halfLine++;

    if (halfLine == 5) {
      if (_stage_ == 6) {
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
          }
        }
      } else if (_stage_ <= 2 || _stage_ == 5 || _stage_ == 8 || _stage_ == 9) {
        if (direction < 0) turnLeft();
        else turnRight();
      }

      halfLine = 0;
    }

  } else halfLine = 0;

  if (_stage_ <= 2 || _stage_ == 5 || _stage_ == 6 || _stage_ == 8) {
    stepperR.setSpeed(2500);
    stepperL.setSpeed(2500);

    stepperL.runSpeed();
    stepperR.runSpeed();
  } else if (_stage_ == 9) {
    stepperR.setSpeed(-2500);
    stepperL.setSpeed(-2500);

    stepperL.runSpeed();
    stepperR.runSpeed();
  }
}