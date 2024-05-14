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

// vrijednosti kalibriranih boja
#define NUM_CALIBRATED_COLORS 3
const Color calibratedColors[NUM_CALIBRATED_COLORS] = {
  {4351, 1981, 368}, // crvena
  {1353, 6939, 1869}, // zelena
  {671, 1906, 780}  // plava
};

// imena kalibriranih boja
const String colorNames[NUM_CALIBRATED_COLORS] = {
  "crvena",
  "zelena",
  "plava"
};

// inicijalizacija senzora
void initColor() {
  color.begin(); // inicializacija senzora boje
  color.drvOn(); // ukljucivanje LED diode
}

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

  //ispis na lcd
  lcd.print(red);
  delay(2000);
  lcd.clear();
  lcd.print(green);
  delay(2000);
  lcd.clear();
  lcd.print(blue);
  delay(2000);
  lcd.clear();
  lcd.print("GOTOV");
  delay(2000);
}

// vraca index ocitane kalibrirane boje
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

// ispise ime ocitane boje na lcd ekranu
void printColor() {
  lcd.clear();
  lcd.print(colorNames[getColor()]);
}