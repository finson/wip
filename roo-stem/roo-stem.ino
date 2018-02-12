#include <Wire.h>

//  Test bed adapted from the Blink and PacketSerial examples. 

#define XLED 4
#define YLED 5
#define ZLED 6

#define VIB_LED LED_BUILTIN
#define VIB_LEVEL 5

int i2cAddress[] = {9,10};

int accPin[] = {A0, A1, A2};
int accOld[] = {0,0,0};
int accNew[] = {0,0,0};

boolean isVib = false;
long vibDone = 0;

// The setup function runs once on reset.

void setup() {

  // initialize digital LED pin as an output.
  
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
  
}

// the loop function runs over and over again forever

void loop() {

  // get the current acceleration values

  long delta = 0;
  for (int i = 0; i<3; i++) {
    accNew[i] = analogRead(accPin[i])-255;
    delta += accNew[i] - accOld[i];
    accOld[i] = accNew[i];
  }

  // If they've changed by more than the trigger magnitude, we're vibrating!
  
  if (delta > VIB_LEVEL) {
    isVib = true;
    vibDone = millis() + 2000;
  } else {
    isVib = !(millis() > vibDone);      
  }

  // Let everyone know what we think is happening

  digitalWrite(VIB_LED, (isVib) ? HIGH : LOW);

  Wire.beginTransmission(i2cAddress[0]);
  Wire.write((char)((isVib) ? HIGH : LOW));
  Wire.endTransmission();
}
 
