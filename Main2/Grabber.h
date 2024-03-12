// otvori hvataljku
void openGrabber() {
  grabberServo.write(GRABBER_OPEN_POSITION);
}

// zatvori hvataljku
void closeGrabber() {
  grabberServo.write(GRABBER_CLOSED_POSITION);
}

// podigne hvataljku
void putHandUp() {
  handServo.write(HAND_UP_POSITION);
}

// polako podigne hvataljku
void putHandUpSlow() {
  for (int i = HAND_DOWN_POSITION; i <= HAND_UP_POSITION; i++) {
    handServo.write(i);
    delay(10);
  }
}

// spusti hvataljku
void putHandDown() {
  handServo.write(HAND_DOWN_POSITION);
}

// polako spusti hvataljku 
void putHandDownSlow() {
  for (int i = HAND_UP_POSITION; i >= HAND_DOWN_POSITION; i--) {
    handServo.write(i);
    delay(10);
  }
}

//pozicionira se u odnosu na puck te ga podize
void pickUpPuck() {
  goForward(1500);
  goForward(-500);
  putGrabberDown();
  delay(1000);
  closeGrabber();
  delay(1000);
  putGrabberUpSlow();
}

//ispusta puck
void letGoOfPuck() {
  putGrabberDownSlow();
  openGrabber();
  delay(1000);
  putGrabberUp();
}