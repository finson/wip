#include <Wire.h>
#include <Adafruit_LSM303.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

//  Central node to read sensors and notify others of results.

#define VIB_LEVEL 0.5     // delta vib threshold
#define VIB_LED_PIN 11

typedef struct {
  int led;
  int pushSense;
  boolean isPressed;
  boolean isLit;
  unsigned long pressActionEnd;
} ArcadeButton;

ArcadeButton button[] = { {12,14,false,false,0}, {10,9,false,false,0} };
int buttonCount = 2;
 
//Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(1);
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified();

int i2cConsumer[] = {9};
int i2cCount = 1;         // only one I2C consumer right now

float accOld[] = {0,0,0};
float accNew[] = {0,0,0};

boolean isVib = false;
unsigned long vibActionEnd = millis();

int loopCount = 0;

void displaySensorDetails(void) {
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

// The setup function runs once on reset.

void setup() {

  // Prepare for console output

  Serial.begin(115200);
  Serial.println("Hello AZ.");

  // initialize digital pins
  
  pinMode(VIB_LED_PIN, OUTPUT);
  digitalWrite(VIB_LED_PIN, HIGH);

  for (int i=0; i<buttonCount; i++) {
    pinMode(button[i].led, OUTPUT);
    digitalWrite(button[i].led, HIGH);

    pinMode(button[i].pushSense, INPUT_PULLUP);
  }

  Serial.println("Pin config done.");
  
  // Configure Wire object for I2C
  
  Wire.begin();
  Wire.setClock(100000);
  
  Serial.println("Wire config done.");

  // Initialise the sensor

  Serial.print("accel.begin() = ");
  Serial.println(accel.begin());
  
  if(!accel.begin()) {
    /* There was a problem detecting the accelerometer ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }

  Serial.println("Sensor config done.");

  // Display some basic information on this sensor
  
  displaySensorDetails();
}

// the loop function runs over and over again forever

void loop() {
  
  sensors_event_t event;

  // get the current acceleration values

  accel.getEvent(&event);
  accNew[0] = event.acceleration.x;
  accNew[1] = event.acceleration.y;
  accNew[2] = event.acceleration.z;
  
  float delta = 0;
  for (int i = 0; i<3; i++) {
    delta += accNew[i] - accOld[i];
    accOld[i] = accNew[i];
  }

  // If accel changed by more than the trigger magnitude, we are (or were) vibrating!
  
  if (abs(delta) > VIB_LEVEL) {
    isVib = true;
    vibActionEnd = millis() + 2000;
  } else {
    isVib = !(millis() > vibActionEnd);      
  }

  // Is (or was) a button pressed?

  for (int i=0; i<buttonCount; i++) {
    int buttonState = digitalRead(button[i].pushSense);
    if (buttonState==0) {
      button[i].isPressed = true;
      button[i].pressActionEnd = millis() + 3000;
      button[i].isLit = true;
    } else {
      button[i].isPressed = !(millis() > button[i].pressActionEnd);
      button[i].isLit = button[i].isPressed;
     }
  }
    
  // Let everyone know what we think is happening
    
  digitalWrite(VIB_LED_PIN, (isVib) ? HIGH : LOW);

  for (int i=0; i<buttonCount; i++) {
    digitalWrite(button[i].led, (button[i].isLit) ? HIGH : LOW);
  }
  
  for (int i = 0; i < i2cCount; i++) {
    Wire.beginTransmission(i2cConsumer[i]);
    Wire.write((char)(isVib));
    Wire.endTransmission();
  }
//
//  Serial.print("Sensor loop ("); Serial.print(loopCount++);Serial.print(") ");
//  for (int i=0;i<3;i++) {
//    Serial.print(accNew[i]);
//    Serial.print(" ");
//  }
  
//  for (int i=0;i<buttonCount;i++) {
//    Serial.print("[ isPressed: ");
//    Serial.print(button[i].isPressed);
//    Serial.print(" isLit: ");
//    Serial.print(button[i].isLit);
//    Serial.print(" }");
////    delay(500);
//  }
//  Serial.println();
}
