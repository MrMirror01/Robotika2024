
struct ToFvalue{
  bool inRangeL;
  int distL = -1;
  bool inRangeR;
  int distR = -1;
};

void initToF(){
  /*pinMode(TOF_L_SHUT_PIN, OUTPUT);
  pinMode(TOF_R_SHUT_PIN, OUTPUT);

  // resetiranje senzora
  digitalWrite(TOF_L_SHUT_PIN, LOW);
  digitalWrite(TOF_R_SHUT_PIN, LOW);
  delay(10);
  digitalWrite(TOF_L_SHUT_PIN, HIGH);
  digitalWrite(TOF_R_SHUT_PIN, HIGH);
  delay(10);*/

  // povezivanje s lijevim senzorom
  /*digitalWrite(TOF_L_SHUT_PIN, HIGH);
  digitalWrite(TOF_R_SHUT_PIN, LOW);
  delay(10);
  tofL.begin(TOF_L_ADDRESS);
  delay(10);*/

  // povezivanje s desnim senzorom
  //digitalWrite(TOF_R_SHUT_PIN, HIGH);
  delay(10);
  tofR.begin(TOF_R_ADDRESS);
  delay(10);
}

// vraca izmjerenu udaljenost u mm (ili -1 ako je izvan dometa)
ToFvalue measureToF(){
  
  VL53L0X_RangingMeasurementData_t measureL;
  VL53L0X_RangingMeasurementData_t measureR;
  ToFvalue val;
  
  tofL.rangingTest(&measureL, false);
  tofR.rangingTest(&measureR, false);

  if (measureL.RangeStatus == 4) val.inRangeL = false;
  else {
    val.inRangeL = true;
    val.distL = measureL.RangeMilliMeter;
  }

  if (measureR.RangeStatus == 4) val.inRangeR = false;
  else {
    val.inRangeR = true;
    val.distR = measureR.RangeMilliMeter;
  }
  
  return val;
}

ToFvalue measureToF_R() {
VL53L0X_RangingMeasurementData_t measureR;
  ToFvalue val;
  
  //tofL.rangingTest(&measureL, false);
  tofR.rangingTest(&measureR, false);

  /*if (measureL.RangeStatus == 4) val.inRangeL = false;
  else {
    val.inRangeL = true;
    val.distL = measureL.RangeMilliMeter;
  }*/

  if (measureR.RangeStatus == 4) val.inRangeR = false;
  else {
    val.inRangeR = true;
    val.distR = measureR.RangeMilliMeter;
  }
  
  return val;
}