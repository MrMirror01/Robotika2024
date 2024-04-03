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
  direction = max(-1, min(1, direction)); // zakvacimo u rasponu [-1, 1]
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

// prati liniju (zauvijek)
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