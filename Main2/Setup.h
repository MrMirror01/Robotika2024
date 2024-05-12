void setup() {
  Serial.begin(9600);

  // inicializacija pinova za pracenje linije
  for (int i = 0; i < 8; i++) {
    pinMode(LINE_FOLLOW_PIN + i, INPUT);
  }
  pinMode(LINE_ENABLE_PIN, OUTPUT);
  digitalWrite(LINE_ENABLE_PIN, HIGH);

  // inicializacija servo motora
  triggerServo.attach(TRIGGER_SERVO_PIN);
  handServo.attach(HAND_SERVO_PIN);
  grabberServo.attach(GRABBER_SERVO_PIN);
  // postavljanje u pocetni polozaj
  putHandUp();
  resetShoot();

  // FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);

  // inicializacija stepper motora
  stepperL.setMaxSpeed(MOVE_MAX_SPEED);
  stepperR.setMaxSpeed(MOVE_MAX_SPEED);
  stepperL.setSpeed(LINE_FOLLOW_MID_SPEED);
  stepperR.setSpeed(LINE_FOLLOW_MID_SPEED);
  stepperL.setAcceleration(ACCELERATION);
  stepperR.setAcceleration(ACCELERATION);

  // inicializacija lcd ekrana
  lcd.init();
  // ukljucivanje backlight-a na lcd-u
  lcd.backlight();
  lcd.print("Epic LCD");

  initColor();

  initToF();

  /*while (true) {
    ToFvalue val = measureToF();
    Serial.print(val.distL);
    Serial.print(" ");
    Serial.println(val.distR);
  }*/

  
  delay(1000);
}