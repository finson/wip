#include <Wire.h>

//  Central node to read sensors and notify others of results.

#define VIB_LED LED_BUILTIN
#define VIB_LEVEL 5
#define VIB_ZERO_OFFSET (-377)

int i2cAddress[] = {9,10};

int accPin[] = {A0, A1, A2};
int accOld[] = {0,0,0};
int accNew[] = {0,0,0};

boolean isVib = false;
long vibDone = 0;

// The setup function runs once on reset.

void setup() {

  // initialize digital pins as outputs.
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // initialize analog pins as inputs

  analogReference(DEFAULT);
  for (int i=0; i<3; i++) {
    pinMode(accPin[i],INPUT);
  }

  // Configure Wire stream for I2C
  
  Wire.begin();
  Wire.setClock(100000);

  // Initialize accelerometer history
  
  for (int i = 0; i<3; i++) {
    accOld[i] = analogRead(accPin[i])+VIB_ZERO_OFFSET;
  }

  // Prepare for debug output
  
//  Serial.begin(115200);

}

// the loop function runs over and over again forever

void loop() {

  // get the current acceleration values

  int delta = 0;
  for (int i = 0; i<3; i++) {
    delay(100);
    accNew[i] = analogRead(accPin[i])+VIB_ZERO_OFFSET;
//    Serial.print(accNew[i]);
//    Serial.print(" ");
    delta += accNew[i] - accOld[i];
    accOld[i] = accNew[i];
  }

  // If they've changed by more than the trigger magnitude, we're vibrating!
  
  if (abs(delta) > VIB_LEVEL) {
    isVib = true;
    vibDone = millis() + 2000;
  } else {
    isVib = !(millis() > vibDone);      
  }

//  Serial.print(" ");
//  Serial.print(delta);
//  Serial.print(" ");
//  Serial.println(isVib);

  // Let everyone know what we think is happening

  digitalWrite(VIB_LED, (isVib) ? HIGH : LOW);
//
//  Wire.beginTransmission(i2cAddress[0]);
//  Wire.write((char)((isVib) ? HIGH : LOW));
//  Wire.endTransmission();
}
 
