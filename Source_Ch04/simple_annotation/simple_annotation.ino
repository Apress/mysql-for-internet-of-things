/**
  Example of simple annotation.
  
  This project demonstrates how to save data to a 
  microSD card as a log file with sting annotation.
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

// Simulated sensor reading method
float read_sensor(int sensor_num) {
  if (sensor_num == 1) {
    return 90.125 + random(20)/10.00;       // sensor #1
  } else if (sensor_num == 2) {
    return 14.512313 + random(100)/100.00;  // sensor #2
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

void loop() {
  delay(2000);
  log_file = SD.open("data_log.txt", FILE_WRITE);
  if (log_file) {
    Serial.println("Log file open.");
    strData = String("Temperature sensor: ");
    strData += read_sensor(1);
    strData += " "; 
    delay(1000);
    strData += ", Barometric sensor: ";
    strData += read_sensor(2);
    log_file.println(strData);
    Serial.print("Data written: ");
    Serial.println(strData);
    log_file.close();
    Serial.println("Log file closed.");
  } else {
    Serial.println("ERROR: Cannot open file for reading.");
  }
}
