/**
  Example of derived or calculated columns annotation.
  
  This project demonstrates how to save data to a 
  microSD card as a log file with additional column annotation.
*/
#include <SPI.h>
#include <SD.h>
#include <String.h>

// Pin assignment for Arduino Ethernet shield
//#define SD_PIN 4
// Pin assignment for Sparkfun microSD shield
#define SD_PIN 8
// Pin assignment for Adafruit Data Logging shield 
//#define SD_PIN 10

File log_file;   // file handle
String strData;  // storage for string
float blood_oxygen;

// Simulated sensor reading method
float read_sensor(int sensor_num) {
  if (sensor_num == 1) {
    return 90.125 + random(20)/10.00;       // sensor #1
  } else if (sensor_num == 2) {
    return 94.512313 + random(100)/100.00;  // sensor #2
  } else if (sensor_num == 3) {
    return 45.6675 + random(100)/100.00;    // sensor #3
  } else {
    if (random(25) >= 5) {
      return (float)random(14)/10.00;
    } else {
      return -(float)random(14)/10.00;
    }
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial); 
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_PIN)) {
    Serial.println("ERROR!");
    return;
  }
  Serial.println("ready.");

  if (SD.remove("data_log.txt")) {
    Serial.println("file removed");
  }

  // initiate random seed
  randomSeed(analogRead(0)); 
}

float oldVal = 67.123;
float newVal = 0.00;
float weight = 0.00;

void loop() {
  delay(2000);
  log_file = SD.open("data_log.txt", FILE_WRITE);
  if (log_file) {
    strData = String("Blood oxygen: ");
    blood_oxygen = read_sensor(2);
    strData += blood_oxygen;
    strData += ", Calibrated: ";
    
    // calculated column adjusting for calibration
    strData += String(blood_oxygen + 0.785, 2);
    strData += ", Weight: ";
    weight = read_sensor(3);
    strData += weight;
    
    // calculated column for number of objects
    strData += ", Avg weight: ";
    strData += String((weight/4.0), 4);
    
    // Calculating change since last read
    strData += ", Volts: ";
    newVal = oldVal+read_sensor(4);
    strData += String(newVal, 2);
    strData += ", Delta: ";
    strData += String(newVal-oldVal,3);
    oldVal = newVal;
    
    log_file.println(strData);
    log_file.close();
  } else {
    Serial.println("Cannot open file for reading.");
  }
}
