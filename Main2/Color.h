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

// inicijalizacija senzora
void initColor() {
  color.begin(); // inicializacija senzora boje
  color.drvOn(); // ukljucivanje LED diode
}

// procita boju te spremi vrijednosti u niz sensorValues
void readColor(){
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

  Serial.print(red);
  Serial.print(" ");
  Serial.print(green);
  Serial.print(" ");
  Serial.println(blue);
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