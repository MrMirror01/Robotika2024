// procita boju te spremi vrijednosti u niz sensorValues
void readColor(){
  lcd.clear();
  color.startMeasurement(); //begin a measurement
  
  //wait till data is available
  bool rdy = false;
  while(!rdy){
    delay(5);
    rdy = color.dataReady();
  }

  color.readRawValues(sensorValues);

  //int red = sensorValues[AS726x_RED];
  //int green = sensorValues[AS726x_GREEN];
  //int blue = sensorValues[AS726x_BLUE];
}