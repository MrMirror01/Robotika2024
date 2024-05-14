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

void modul4Puck(){
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
  }
  else {
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
  goForward(4250);
  alignToWall();
  rotateRight(1500);

  goForwardUntilEnd();
  delay(2000);

  // -- CRVENA --
  if (colorIdx == 0) {
    goForward(3500);
    alignToWall();
    letGoOfPuck();
    rotateRight(1500);
    goForward(1000);
    turnLeft();
    turnRight();
  } // -- ZELENA --
  else if (colorIdx == 1) {
    goForward(1500);
    alignToWall();
    turnRight();
    goForward(3200);
    rotateLeft(1500);
    letGoOfPuck();
    rotateRight(1500);
    goForward(1000);
    turnLeft();
    turnRight();
  } // -- PLAVA --
  else if (colorIdx == 2) {
    goForward(1500);
    alignToWall();
    turnRight();
    goForward(6600);
    rotateLeft(1500);
    letGoOfPuck();
    rotateRight(1500);
    goForward(1000);
    turnLeft();
    turnRight();
  }

  goForwardUntilEnd();
  

  // -- CURLING --
  turnLeft();
  turnAroundRight();

  // puck 1
  followLineUntilEnd();
  turnAroundRight();
  goForward(-2750);
  shoot();
  delay(500);

  goForward(3000);
  alignToWall();
  turnAroundRight();
  followLineUntilEnd();

  // puck 2
  goForward(1000);
  rotateLeft(450);
  goForward(1200);  
  pickUpPuck();
  goForward(-750);
  rotateRight(450);
  goForward(-3000);
  goForward(1000);
  goForwardUntilEndFast();

  goForward(-6500);
  followLineUntilEnd();

  // puck 3
  goForward(1000);
  rotateRight(450);
  goForward(1200);  
  pickUpPuck();
  goForward(-750);
  rotateLeft(450);
  goForward(-3000);
  goForward(1000);
  goForwardUntilEndFast();
}

void loop() {
  // put your main code here, to run repeatedly:
  modul5();

  while (true) delay(1000);
}
