//ispucavanje
void shoot() {
  triggerServo.write(SHOOT_TRIGGER_POSITION);
}

// postavlja servo za ispucavanje u pocetni polozaj
void resetShoot() {
  triggerServo.write(SHOOT_RESET_POSITION);
}