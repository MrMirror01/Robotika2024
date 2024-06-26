#include "Init.h"
#include "Movement.h"
#include "LineFollow.h"
#include "Ultrasonic.h"
#include "ToF.h"
#include "Grabber.h"
#include "Color.h"
#include "Shooting.h"
#include "Setup.h"

/*
__/\\\\\\\\\\\\\\\__/\\\\\\\\\\\_______/\\\\\______        
 _\///////\\\/////__\/////\\\///______/\\\///\\\____       
  _______\/\\\___________\/\\\_______/\\\/__\///\\\__      
   _______\/\\\___________\/\\\______/\\\______\//\\\_     
    _______\/\\\___________\/\\\_____\/\\\_______\/\\\_    
     _______\/\\\___________\/\\\_____\//\\\______/\\\__   
      _______\/\\\___________\/\\\______\///\\\__/\\\____  
       _______\/\\\________/\\\\\\\\\\\____\///\\\\\/_____ 
        _______\///________\///////////_______\/////_______
*/

void modul4() {
  //goForward(2500);
  goForwardUntilTurn();
  alignForward();
  rotateRight(750);
  goForward(-2300);
  goForwardUntilTurn();
  delay(500);
  alignForward();
  rotateLeft(750);
  goForward(-2500);

  goForward(900);

  followLineSteps(1150);
  printColor();
  delay(1000);

  followLineSteps(750);
  printColor();
  delay(1000);


  followLineSteps(750);
  printColor();
  delay(1000);

  followLineUntilEnd();

  printDistance();
}

void modul4Puck() {
  pickUpPuck();
  letGoOfPuck();
  turnAroundRight();
  goForward(-2500);
  shoot();
}

int colorIdx = -1;

void modul5() {
  goForward(1000);
  followLineUntilEnd();
  alignToWall();

  printColor();
  colorIdx = getColor();

  // -- PUCK --
  rotateLeft(180);
  goForward(1000);
  if (!digitalRead(PUCK_SENSOR_PIN)) {
    pickUpPuck();

    rotateRight(220);
  } else {
    goForward(-1000);
    rotateRight(400);
    goForward(1000);
    pickUpPuck();
    rotateLeft(220);
  }

  // -- LABIRINT --
  turnRight();
  goForward(4500);
  rotateRight(1500);
  goForward(4500);
  alignToWall();
  rotateLeft(1500);
  goForward(6500);
  alignToWall();
  rotateLeft(1500);
  goForward(3500);
  alignToWall();
  driveUntilObstacle(5);
  alignToWall();
  rotateRight(1500);

  goForwardUntilEnd();
  delay(1000);

  // -- CRVENA --
  if (colorIdx == 0) {
    goForward(2500);
    alignToWall();
    driveUntilObstacle(30);
    alignToWall();
    rotateLeft(100);
    letGoOfPuck();
    rotateRight(100);
    rotateRight(1500);
    goForward(1000);
    turnLeft();
    turnRight();
  }  // -- ZELENA --
  else if (colorIdx == 1) {
    goForward(1500);
    alignToWall();
    turnRight();
    goForward(3050);
    rotateLeft(1500);
    letGoOfPuck();
    rotateRight(1500);
    goForward(1000);
    turnLeft();
    turnRight();
  }  // -- PLAVA --
  else if (colorIdx == 2) {
    goForward(1500);
    alignToWall();
    turnRight();
    goForward(6750);
    rotateLeft(1500);
    letGoOfPuck();
    rotateRight(1500);
    goForward(500);
    turnLeft();
    turnRight();
  }

  goForwardUntilEnd();



  // -- CURLING --
  turnLeft();
  alignToWall();
  driveUntilObstacle(5);
  alignToWall();
  turnAroundRight();

  // puck 1
  followLineUntilEnd();
  turnAroundRight();
  goForward(-3750);
  shoot();
  delay(500);

  goForward(3000);
  driveUntilObstacle(5);
  alignToWall();
  turnAroundRight();
  followLineUntilEnd();

  // puck 2
  goForward(1000);
  rotateLeft(500);
  goForward(1200);
  pickUpPuck();
  goForward(-750);
  rotateRight(500);
  goForward(-3000);
  goForwardUntilEndFast();

  turnAroundLeft();
  goForward(3500);
  rotateLeft(150);
  followLineSteps(2000);
  alignToWall();
  driveUntilObstacle(5);
  alignToWall();
  turnAroundRight();

  followLineUntilEnd();

  // puck 3
  goForward(1000);
  rotateRight(575);
  goForward(1200);
  pickUpPuck();
  rotateLeft(575);
  goForward(-3100);
  goForwardUntilEndFast();
}

bool handUp = true;
bool grabberOpen = true;

float x = 0, y = 0;
float speed = 0;

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');

    if (command.length() == 2) {
      if (command == "B0") {
        if (handUp) putHandUpSlow();
        else putHandDown();

        handUp = !handUp;
      }
      else if (command == "B1") {
        shoot();
        delay(1000);
        resetShoot();
      }
      else {
        if (grabberOpen) closeGrabber();
        else openGrabber();

        grabberOpen = !grabberOpen;
      }
    }
    else {
      command = command.substring(3, command.length());
      
      double angle = 0;
      speed = 0;

      int i;
      for (i = 0; command[i] != ','; i++){
        angle *= 10;
        angle += command[i] - '0';
      }

      speed += command[i + 1] - '0';
      speed += 0.1 * (command[i + 3] - '0');

      angle = angle / 360 * 2 * 3.1415;

      x = cos(angle);
      y = sin(angle);
    }
    
  }

  stepperL.setSpeed(speed * 500 * (y + x));
  stepperR.setSpeed(speed * 500 * (y - x));

  stepperL.runSpeed();
  stepperR.runSpeed();
}