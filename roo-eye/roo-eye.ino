#include <Wire.h>

  //  Test bed adapted from the Blink and PacketSerial examples. 
  //  This is the Device Agent module for the jumperoo eye display.

#define LED_A 0
#define LED_B 1
#define LED_C 2
#define LED_D 3
#define LED_E 4

#define I2C_DEVICE 10

void receiveEvent(int bytes) { 
  boolean isVib = (boolean)Wire.read();
  digitalWrite(LED_BUILTIN, (isVib)? HIGH : LOW);

  if (isVib) {
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(LED_A, HIGH);
    digitalWrite(LED_B, HIGH);
    digitalWrite(LED_C, HIGH);
    digitalWrite(LED_D, HIGH);
    digitalWrite(LED_E, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(LED_A, LOW);
    digitalWrite(LED_B, LOW);
    digitalWrite(LED_C, LOW);
    digitalWrite(LED_D, LOW);
    digitalWrite(LED_E, LOW);
  }
}

// The setup function runs once on reset.

void setup() {
  
  // initialize digital pins as an outputs.
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  pinMode(LED_A, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(LED_C, OUTPUT);
  pinMode(LED_D, OUTPUT);
  pinMode(LED_E, OUTPUT);

  // Configure Wire stream for I2C
  
  Wire.begin(I2C_DEVICE);
  Wire.setClock(100000);
  Wire.onReceive(receiveEvent); 
}

// the loop function runs over and over again forever

void loop() {
}
