#include <PacketSerial.h>

/*
  Test bed adapted from the Blink and PacketSerial examples. 
*/

#define LED_A 0
#define LED_B 1
#define LED_C 2
#define LED_D 3
#define LED_E 4

uint8_t packet[64];

PacketSerial myPacketSerial;

void onPacketReceived(const uint8_t* packet, size_t size) {
  if (packet[0] == 0) {
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(LED_A, HIGH);
    digitalWrite(LED_B, HIGH);
    digitalWrite(LED_C, HIGH);
    digitalWrite(LED_D, HIGH);
    digitalWrite(LED_E, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
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
  pinMode(LED_A, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(LED_C, OUTPUT);
  pinMode(LED_D, OUTPUT);
  pinMode(LED_E, OUTPUT);

  Serial1.begin(9600);
  myPacketSerial.setStream(&Serial1);
  myPacketSerial.setPacketHandler(&onPacketReceived);

  digitalWrite(LED_BUILTIN, HIGH);

}

// the loop function runs over and over again forever

void loop() {

  myPacketSerial.update();

}
