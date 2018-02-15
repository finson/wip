#include <Wire.h>
#include <Adafruit_LSM303.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>


//  Central node to read sensors and notify others of results.

#define VIB_LED LED_BUILTIN
#define BUTTON_LED 11
#define VIB_LEVEL 0.5

Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(1);

int i2cAddress[] = {9,10};

float accOld[] = {0,0,0};
float accNew[] = {0,0,0};

boolean isVib = false;
long vibDone = millis();

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

  delay(100);
  Serial.begin(115200);

  // initialize digital pins as outputs.
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  
  pinMode(BUTTON_LED, OUTPUT);
  digitalWrite(BUTTON_LED, HIGH);

  // Configure Wire object for I2C
  
  Wire.begin();
  Wire.setClock(100000);

  // Initialise the sensor

  if(!accel.begin()) {
    /* There was a problem detecting the accelerometer ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }

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
    Serial.print(accNew[i]);
    Serial.print(" ");
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

  Serial.print(" ");
  Serial.print(isVib);
  Serial.print(" ");
  Serial.println(delta);

  // Let everyone know what we think is happening

  digitalWrite(VIB_LED, (isVib) ? HIGH : LOW);
  digitalWrite(BUTTON_LED, (isVib) ? HIGH : LOW);

  Wire.beginTransmission(i2cAddress[0]);
  Wire.write((char)((isVib) ? HIGH : LOW));
  Wire.endTransmission();
}
 
