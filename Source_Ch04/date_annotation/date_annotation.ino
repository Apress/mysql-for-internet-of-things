/**
  Example of date annotation.
  
  This project demonstrates how to save data to a 
  microSD card as a log file with sting and date annotation.
*/
#include <SPI.h>
#include <SD.h>
#include <String.h>
#include <Wire.h>
#include "RTClib.h"

// Pin assignment for Arduino Ethernet shield
//#define SD_PIN 4
// Pin assignment for Sparkfun microSD shield
#define SD_PIN 8
// Pin assignment for Adafruit Data Logging shield 
//#define SD_PIN 10

File log_file;   // file handle
String strData;  // storage for string
RTC_DS1307 rtc;  // real time clock

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

  rtc.begin();
  if (!rtc.begin()) {
    Serial.println("Couldn't find the RTC module.");
    while (1);
  }

  if (! rtc.isrunning()) {
    Serial.println("The RTC module is not working.");
  } else {
    // Comment out this line after the first run, it is only needed for setting
    // the date and time for the first time.
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // initiate random seed
  randomSeed(analogRead(0)); 
}

String get_datetime() {
  DateTime now = rtc.now();
  String dateStr = String(now.day());
  dateStr += "/";
  dateStr += now.month();
  dateStr += "/";
  dateStr += now.year();
  dateStr += " ";
  dateStr += String(now.hour());
  dateStr += ":";
  dateStr += String(now.minute());
  dateStr += ":";
  dateStr += String(now.second());
  return dateStr;
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
    strData += " ";
    strData += get_datetime();
    log_file.println(strData);
    Serial.print("Data written: ");
    Serial.println(strData);
    log_file.close();
    Serial.println("Log file closed.");
    delay(2000);
  } else {
    Serial.println("ERROR: Cannot open file for reading.");
  }
}
