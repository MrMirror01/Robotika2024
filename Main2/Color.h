/*
sensorValues[AS726x_VIOLET]
sensorValues[AS726x_BLUE]
sensorValues[AS726x_GREEN]
sensorValues[AS726x_YELLOW]
sensorValues[AS726x_ORANGE]
sensorValues[AS726x_RED]
*/

struct Color {
  float r, g, b;
};

// ocitane boje
#define NUM_CALIBRATED_COLORS 3
const Color calibratedColors[NUM_CALIBRATED_COLORS] = {
  {5000, 0, 0}, // crvena
  {0, 5000, 0}, // zelena
  {0, 0, 5000}  // plava
};

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

  color.readCalibratedValues(sensorValues);

  float red = sensorValues[AS726x_RED];
  float green = sensorValues[AS726x_GREEN];
  float blue = sensorValues[AS726x_BLUE];

  lcd.print(red);
  lcd.print(" ");
  lcd.print(green);
  lcd.print(" ");
  lcd.print(blue);
}

int getColor(){
  color.startMeasurement(); //begin a measurement
  
  //wait till data is available
  bool rdy = false;
  while(!rdy){
    delay(5);
    rdy = color.dataReady();
  }

  color.readCalibratedValues(sensorValues);

  float red = sensorValues[AS726x_RED];
  float green = sensorValues[AS726x_GREEN];
  float blue = sensorValues[AS726x_BLUE];

  int idx = 0;
  float distance = 1e12;
  for (int i = 0; i < NUM_CALIBRATED_COLORS; i++){
    float dist = 
      pow(red - calibratedColors[i].r, 2) +
      pow(green - calibratedColors[i].g, 2) +
      pow(blue - calibratedColors[i].b, 2);

    if (dist < distance) {
      distance = dist;
      idx = i;
    }
  }

  return idx;
}