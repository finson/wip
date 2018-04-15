#include <Wire.h>

  //  Test bed adapted from the Blink and PacketSerial examples. 
  //  This is the Device Agent module for the jumperoo eye display.

#define LED_1 14
#define LED_3 10
#define LED_5 9
#define LED_7 12

int ledPin[] = {LED_BUILTIN,LED_1,LED_3,LED_5,LED_7};
int ledCount = 5;

#define I2C_DEVICE 9  // The I2C address of this teensy

void receiveEvent(int bytes) { 
//  boolean isVib = (boolean)(bytes != 0);
  boolean isVib = (boolean)Wire.read();
  Serial.print("isVib: ");Serial.println(isVib);
  for (int i=0; i<ledCount; i++) {
    digitalWrite(ledPin[i], (isVib) ? HIGH : LOW);
  }
}

// The setup function runs once on reset.

void setup() {
  Serial.print("roo-eye setup.");
  
  // initialize digital pins as an outputs.

  for (int i=0; i<ledCount; i++) {
    pinMode(ledPin[i], OUTPUT);
    digitalWrite(ledPin[i], HIGH);
  }

  // Configure Wire stream for I2C
  
  Wire.begin(I2C_DEVICE);
//  Wire.setClock(100000);
  Wire.onReceive(receiveEvent); 
}

// the loop function runs over and over again forever

void loop() {
}
