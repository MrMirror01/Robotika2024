//#include <FastLED.h>
#include <AccelStepper.h>
#include <NewPing.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

//#define NUM_LEDS 24
//#define LED_PIN 12
#define LINE_FOLLOW_PIN 52
#define LINE_FOLLOW_ENABLE_PIN 36
//CRGB leds[NUM_LEDS];

#define STEP_DIR_PIN_R 8
#define STEP_PIN_R 9
#define STEP_DIR_PIN_L 7
#define STEP_PIN_L 6
#define MOTOR_INTERFACE_TYPE 1
AccelStepper stepperL = AccelStepper(MOTOR_INTERFACE_TYPE, STEP_PIN_L, STEP_DIR_PIN_L);
AccelStepper stepperR = AccelStepper(MOTOR_INTERFACE_TYPE, STEP_PIN_R, STEP_DIR_PIN_R);

#define SONAR_TRIGGER_PIN_L 43
#define SONAR_ECHO_PIN_L 41
#define SONAR_TRIGGER_PIN_R 23
#define SONAR_ECHO_PIN_R 25
#define MAX_DISTANCE 100
NewPing sonarL(SONAR_TRIGGER_PIN_L, SONAR_ECHO_PIN_L, MAX_DISTANCE);
NewPing sonarR(SONAR_TRIGGER_PIN_R, SONAR_ECHO_PIN_R, MAX_DISTANCE);

#define TRIGGER_SERVO_PIN 49
Servo triggerServo;
#define GRABBER_SERVO_UD_PIN 45
#define GRABBER_SERVO_OPENCLOSE_PIN 47
Servo grabberServoUD;
Servo grabberServoOpenClose;

LiquidCrystal_I2C lcd(0x27, 16, 2);