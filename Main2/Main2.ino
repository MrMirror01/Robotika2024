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

void loop() {
  // put your main code here, to run repeatedly:
  /*followLineUntilEndWithTurning();
  alignToWall();
  pickUpPuck();
  turnAroundRight();
  followLineUntilEndWithTurning();
  letGoOfPuck();
  turnAroundRight();
  goForward(-2000);
  shoot();*/
  while (true) {
    /*ToFvalue value = measureToF_R();
    Serial.print(value.distL);
    Serial.print(" ");
    Serial.println(value.distR);*/
    readColor();
    delay(500);
  }


  while (true) delay(1000);
}
