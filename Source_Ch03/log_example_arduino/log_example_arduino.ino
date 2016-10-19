/**
  Example Arduino SD card log file.
  
  This project demonstrates how to save data to a 
  microSD card as a log file and read it.
*/
#include <SPI.h>
#include <SD.h>
#include <String.h>

// Pin assignment for Arduino Ethernet shield
#define SD_PIN 4
// Pin assignment for Sparkfun microSD shield
//#define SD_PIN 8
// Pin assignment for Adafruit Data Logging shield 
//#define SD_PIN 10

File log_file;

void setup() {
  char c = ' ';
  char number[4];
  int i = 0;
  int value = 0;
  String text_string;

  Serial.begin(115200);
  while (!Serial); // wait for serial to load

  Serial.print("Initializing SD card...");

  if (!SD.begin(SD_PIN)) {
    Serial.println("ERROR!");
    return;
  }
  Serial.println("done.");

//  if (SD.remove("log.txt")) {
//    Serial.println("file removed");
//  }

  log_file = SD.open("log.txt", FILE_WRITE);
  if (log_file) {
    for (int i=0; i < 10; i++) {
      text_string = String(i); 
      text_string += ", Example row: ";
      text_string += String(i+1);
      log_file.println(text_string);
    }
    log_file.close();
  } else {
    Serial.println("Cannot open file for writing.");
  }

  log_file = SD.open("log.txt");
  if (log_file) {
    // Read one row at a time.
    while (log_file.available()) {
      text_string = String("");
      
      // Read first column
      i = 0;
      while ((c != ',') && (i < 4)) {
        c = log_file.read();
        text_string += c;
        if (c != ',') {
          number[i] = c;
        }
        i++;
      }
      number[i] = '\0';
      value = atoi(number);

      // Read second column
      c = ' ';
      while (c != '\n') {
        c = log_file.read();
        text_string += c;
      }
      // If value > 5, print the row
      if (value > 5) {
        Serial.print("> ");
        Serial.print(text_string);
      }
    }
    // close the file:
    log_file.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("Cannot open file for reading");
  }
}

void loop() {
  // do nothing
}

