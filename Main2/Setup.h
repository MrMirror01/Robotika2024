void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 8; i++) {
    pinMode(LINE_FOLLOW_PIN + i, INPUT);
  }
  //pinMode(LINE_FOLLOW_ENABLE_PIN, OUTPUT);
  //digitalWrite(LINE_FOLLOW_ENABLE_PIN, HIGH);

  triggerServo.attach(TRIGGER_SERVO_PIN);

  handServo.attach(HAND_SERVO_PIN);
  grabberServo.attach(GRABBER_SERVO_PIN);

  //FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  stepperL.setMaxSpeed(MOVE_MAX_SPEED);
  stepperR.setMaxSpeed(MOVE_MAX_SPEED);
  stepperL.setSpeed(LINE_FOLLOW_MID_SPEED);
  stepperR.setSpeed(LINE_FOLLOW_MID_SPEED);
  stepperL.setAcceleration(ACCELERATION);
  stepperR.setAcceleration(ACCELERATION);

  lcd.init();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.print("Epic LCD");

  color.begin();

  putHandUp();
  resetShoot();
  delay(1000);

  followLineForever();
}