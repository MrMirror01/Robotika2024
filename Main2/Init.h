//#include <FastLED.h>
#include <AccelStepper.h>
#include <NewPing.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "Adafruit_AS726x.h"
#include <Adafruit_VL53L0X.h>

// konstantne vrijednosti
#define MOVE_MAX_SPEED 5000 // maksimalna brzina kod animiranog kretanja
#define ACCELERATION 2500 // akceleracija motora pri animiranom kretanju
#define LINE_FOLLOW_MID_SPEED 3500 // srednja brzina pracenja linije
#define ALIGN_STEPS 2000 // broj koraka potreban za pomicanje straznjeg djela robota na poziciju senzora za liniju
#define RIGHT_ANGLE_STEPS 1500 // broj koraka potreban za rotaciju od 90 stupnjeva

#define HAND_UP_POSITION 60
#define HAND_DOWN_POSITION 0
#define GRABBER_OPEN_POSITION 140
#define GRABBER_CLOSED_POSITION 0

#define SHOOT_RESET_POSITION 170
#define SHOOT_TRIGGER_POSITION 110

// koeficijenti za PID regulaciju
#define Kp 2 // proporcionalni koeficijent TIP: premalo = lose u zavojima; previse = oscilacije
#define Ki 0 // integracijski koeficijent (ispravljanje gresaka s vremenom) TIP: premalo = losa preciznost; previse = nestabilni sustav
#define Kd 0 // derivacijski koeficijent (ostri zavoji) TIP: premalo = skakuce; previse = sporo i dosadno



//#define NUM_LEDS 24
//#define LED_PIN 12
//CRGB leds[NUM_LEDS];

#define LINE_FOLLOW_PIN A0
#define LINE_ENABLE_PIN 53
#define LINE_TRESHOLD 750

// stepperi
#define STEP_DIR_PIN_R 8
#define STEP_PIN_R 9
#define STEP_DIR_PIN_L 7
#define STEP_PIN_L 6
#define MOTOR_INTERFACE_TYPE 1
AccelStepper stepperL = AccelStepper(MOTOR_INTERFACE_TYPE, STEP_PIN_L, STEP_DIR_PIN_L);
AccelStepper stepperR = AccelStepper(MOTOR_INTERFACE_TYPE, STEP_PIN_R, STEP_DIR_PIN_R);

// ultrasonicni
#define SONAR_TRIGGER_PIN_L 24
#define SONAR_ECHO_PIN_L 23
#define SONAR_TRIGGER_PIN_R 12
#define SONAR_ECHO_PIN_R 11
#define MAX_DISTANCE 100
NewPing sonarL(SONAR_TRIGGER_PIN_L, SONAR_ECHO_PIN_L, MAX_DISTANCE);
NewPing sonarR(SONAR_TRIGGER_PIN_R, SONAR_ECHO_PIN_R, MAX_DISTANCE);

// servo motori
#define TRIGGER_SERVO_PIN 42
Servo triggerServo;
#define HAND_SERVO_PIN 41
#define GRABBER_SERVO_PIN 10
Servo handServo;
Servo grabberServo;

// lcd
LiquidCrystal_I2C lcd(0x27, 16, 2);

// senzor boje
Adafruit_AS726x color;
float sensorValues[AS726x_NUM_CHANNELS];
#define COLOR_R_SHUT 52

// tof
#define TOF_L_SHUT_PIN 43
#define TOF_R_SHUT_PIN 50
#define TOF_L_ADDRESS 0x35
#define TOF_R_ADDRESS 0x36
Adafruit_VL53L0X tofL = Adafruit_VL53L0X();
Adafruit_VL53L0X tofR = Adafruit_VL53L0X();
