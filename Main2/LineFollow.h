// PID line follow
float P, I, D;
float lastError; // zadnja izmjerena pogreska

float error = 0; // [-1, 1], pogreska ovisno o senzorima koji vide crtu
int numSensors; // broj senzora koji vidi liniju

float direction; // izracunata razlika brzine motora

unsigned long long stepID = 0; // koji je to po redu step

// izracunava gresku ovisno o senzorima koji vide crtu
void calculateLineError(){
  error = 0;
  numSensors = 0;
  for (int i = 0; i < 8; i++) {
    bool sensor = analogRead(LINE_FOLLOW_PIN + i) > LINE_TRESHOLD;
    if (sensor) numSensors++;
    if (i < 4) {
      error += sensor * (4 - i);
    } else {
      error -= sensor * (i - 3);
    }
    //Serial.print(sensor);
    //Serial.print(" ");
  }
  //Serial.println("");
  error /= 10;
  //Serial.println(error);
}

// izracunamo smjer i intenzitet skretanja koristeci PID regulaciju
void calculatePID(){
  P = error; // proporcionalno greski
  I += error; // integral greske
  D = error - lastError; //derivacija greske
  /*if (stepID % 100 == 0)*/ lastError = error;

  direction = Kp * P + Ki * I + Kd * D; // izracunamo smjer i intenzitet skretanja
  //direction = min(1, max(-1, direction)); // zakvacimo u rasponu [-1, 1]
}

// izracuna smjer kretanja te postavlja odgovarajuce brzine motora
void setMotorSpeeds(){
  calculateLineError();
  calculatePID();

  stepperL.setSpeed(LINE_FOLLOW_MID_SPEED * (1. + direction));
  stepperR.setSpeed(LINE_FOLLOW_MID_SPEED * (1. - direction));
}

// prati liniju (zauvijek)
void followLineForever(){
  while (true){
    setMotorSpeeds();

    stepperL.runSpeed();
    stepperR.runSpeed();
  }
}

// prati liniju (dok ne dođe do crne oznake)
void followLineUntilEnd(){
  int cnt = 0;
  while (true){
    setMotorSpeeds();

    if (numSensors == 8){
      cnt++;
    }
    else cnt = 0;

    if (cnt >= 50) return;

    stepperL.runSpeed();
    stepperR.runSpeed();
  }
}

// prati liniju određeni broj koraka
void followLineSteps(int steps){
  int cnt = 0;
  while (true){
    setMotorSpeeds();

    cnt++;

    if (cnt >= steps) return;

    stepperL.runSpeed();
    stepperR.runSpeed();
  }
}

// ide ravno (dok ne dođe do skretanja)
void goForwardUntilTurn(){
  stepperL.setSpeed(1500);
  stepperR.setSpeed(1500);

  int cnt = 0;
  while (true){
    calculateLineError();
    if (numSensors > 4){
      cnt++;
    }
    else cnt = 0;

    if (cnt >= 50) return;

    stepperL.runSpeed();
    stepperR.runSpeed();
  }
}

// ide ravno (dok ne dođe do poprecne oznake)
void goForwardUntilEnd(){
  stepperL.setSpeed(1500);
  stepperR.setSpeed(1500);

  int cnt = 0;
  while (true){
    calculateLineError();
    if (numSensors == 8){
      cnt++;
    }
    else cnt = 0;

    if (cnt >= 100) return;

    stepperL.runSpeed();
    stepperR.runSpeed();
  }
}

void goForwardUntilEndFast() {
  handServo.write(HAND_UP_POSITION + 30);

  float speed = 100;
  goForward(1900);
  stepperL.setSpeed(speed);
  stepperR.setSpeed(speed);

  int cnt = 0;
  while (true){
    //calculateLineError();
    cnt++;

    if (cnt == 5000) {
      grabberServo.write(GRABBER_OPEN_POSITION);
      handServo.write(HAND_UP_POSITION - 20);
    }
    if (cnt >= 6000) return;

    stepperL.runSpeed();
    stepperR.runSpeed();
    speed += 2;
    stepperL.setSpeed((int)speed);
    stepperR.setSpeed((int)speed);
  }

  handServo.write(HAND_UP_POSITION);
}

// prati liniju (dok ne dođe do crne oznake) te skreće animirano kad dođe do pravog kuta
void followLineUntilEndWithTurning(){
  int cnt = 0;
  int cntL = 0, cntR = 0;
  while (true){
    setMotorSpeeds();

    if (numSensors == 8){
      cnt++;
    }
    else cnt = 0;

    if (numSensors > 4 && error > 0) {
      cntR++;
    }
    else cntR = 0;

    if (numSensors > 4 && error < 0) {
      cntL++;
    }
    else cntL = 0;

    if (cnt >= 50) return;
    if (cntR >= 100) turnRight();
    if (cntL >= 100) turnLeft();

    stepperL.runSpeed();
    stepperR.runSpeed();
  }
}