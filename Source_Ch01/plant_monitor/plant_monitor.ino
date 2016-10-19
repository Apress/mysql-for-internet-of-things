/*
 Simple Plant Monitor Example

 Display the value of the soil moisture sensor and threshold status.

*/

// Thresholds for sensor to detect wet/dry conditions. Adjust these
// to match your soil characteristics.
int upper = 400;
int lower = 250;

// Pin for reading the sensor
int sensorPin = A0;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial); 
  Serial.println("Simple plant monitor");
  Serial.println("raw value, moisture state");
}

void loop() {
  int value;
  
  // Read the sensor
  value = analogRead(sensorPin);
  
  Serial.print(value); 
  Serial.print(",");

  // If value is less than lower threshold, soil is dry else if it
  // is greater than upper threshold, it is wet, else all is well.
  if (value <= lower) {
    Serial.print("dry");
  } else if (value >= upper) {
    Serial.print("wet");
  } else {
    Serial.print("ok");
  }
  Serial.println();
  delay(250);
}

