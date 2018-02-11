#include <Wire.h>
//#include <PacketSerial.h>

/*
  Test bed adapted from the Blink and PacketSerial examples. 
*/

#define INTERVAL 500
#define VIB_PIN 13

//PacketSerial myPacketSerial;
uint8_t packet[64];
uint32_t sentTime;

boolean isVib = false;

// The setup function runs once on reset.

void setup() {
  // initialize LED pin as an output.
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, packet[0]);
  pinMode(VIB_PIN, INPUT);

  Serial.begin(115200);
  sentTime = millis();

  

}

// the loop function runs over and over again forever

void loop() {

  isVib = (digitalRead(VIB_PIN) != LOW);
  if (isVib) {
    Serial.print(millis());
    Serial.print(" ");
    Serial.println("I'm vibrating!");
  }
  

//  if (millis()-sentTime > 1000) {
//    sentTime = millis();
//    packet[0] = ~packet[0];
//    digitalWrite(LED_BUILTIN, packet[0]);
//    myPacketSerial.send(packet, 2);
//  }
//  
//    myPacketSerial.update();
}
