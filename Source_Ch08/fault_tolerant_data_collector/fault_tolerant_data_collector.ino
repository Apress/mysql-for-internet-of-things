/**
  Example Arduino fault tolerant data collector
  
  This project demonstrates how to save data to a 
  microSD card as a cache should the node fail to
  save data to a MySQL database server.
*/
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <Ethernet.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include "RTClib.h"

// Pin assignment for Arduino Ethernet shield
#define SD_PIN 4
// Pin assignment for Sparkfun microSD shield
//#define SD_PIN 8
// Pin assignment for Adafruit Data Logging shield 
//#define SD_PIN 10

#define LOGFILE "cache.txt"  // name of log file for caching data

int sensorPin = 0; // Pin for reading the sensor

RTC_DS1307 rtc;  // real time clock

byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress server_addr(10,0,1,8);  // Supply the IP of the MySQL *server* here

char user[] = "root";              // can be anything but the user must have
char password[] = "secret";        // access rights to connect (host must permit it)

EthernetClient client;
MySQL_Connection conn((Client *)&client);

// Get the date and time from the RTC
String get_datetime() {
  DateTime now = rtc.now();
  String dateStr = String(now.year());
  dateStr += "-";
  dateStr += now.month();
  dateStr += "-";
  dateStr += now.day();
  dateStr += " ";
  dateStr += String(now.hour());
  dateStr += ":";
  dateStr += String(now.minute());
  dateStr += ":";
  dateStr += String(now.second());
  return dateStr;
}

// Save the data to the SD card for later processing
// Note: What happens when the disk is full?
bool cache_data(int value) {
  File log_file;

  String row = String(value);
  log_file = SD.open(LOGFILE, FILE_WRITE);
  if (log_file) {
    // Get datetime
    row += ",";
    row += get_datetime();
    // Save data to the file
    log_file.println(row);
    log_file.close();
    Serial.println("Data cached to log file.");
    return true;
  }
  Serial.println("ERROR: Cannot open file for writing.");
  return false;
}

// Save the data to the database
bool write_data(int val, const char *dateStr) {
  String queryStr = String("INSERT INTO plant_moisture.plants VALUES (NULL,");

  queryStr += String(val);
  queryStr += ",";
  if (dateStr != "NULL") queryStr += "'";  // quote string for values
  queryStr += dateStr;
  if (dateStr != "NULL") queryStr += "'";  // quote string for values
  queryStr += ")";
  // write the data here
  Serial.println(queryStr);
  // Create an instance of the cursor passing in the connection
  MySQL_Cursor *cur = new MySQL_Cursor(&conn);
  cur->execute(queryStr.c_str());
  delete cur;
}

// Read the cache file on disk and write rows found to the database
bool dump_cache() {
  File log_file;
  char char_read;
  char buffer[128];
  int i = 0;
  int comma_pos = 0;
  int val = 0;

  // if no file, no cache!
  if (!SD.exists(LOGFILE)) return true;
  log_file = SD.open(LOGFILE);
  if (log_file) {
    // Read one row at a time.
    while (log_file.available()) {
      char_read = log_file.read();
      if (char_read != '\n') {
        // look for fist value
        if (char_read == ',') {
          buffer[i] = '\0';
          val = atoi(buffer);
          i = 0;
        } else {
          buffer[i] = char_read;
          i++; 
        }
      } else {
        buffer[i] = '\0';
        i = 0;
        // Write the row to the database
        write_data(val, buffer);
      }
    }  
    // close the file:
    log_file.close();
    // Now remove the file
    if (SD.remove(LOGFILE)) {
      Serial.println("Cache emptied. File removed.");
    }
    return true;
  }
  // if the file didn't open, print an error:
  Serial.println("ERROR: Cannot open file for reading");
  return false;
}

void setup() {

  Serial.begin(115200);
  while (!Serial); // wait for serial to load

  // Turn on the SPI for the SD card (Arduino Ethernet)
  // Note: required for Arduino Mega!
  pinMode(10,OUTPUT);     
  digitalWrite(10,HIGH);
  
  // Start the RTC
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

  // Setup the SD card
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_PIN)) {
    Serial.println("ERROR: Cannot initialize SD card");
    return;
  }
  Serial.println("SD card is ready.");
  // Now remove the file
  if (SD.remove(LOGFILE)) {
    Serial.println("Cache emptied. File removed.");
  }

  // Setup the network connection
  Ethernet.begin(mac_addr);

  delay(1000);
  // Test the connection
  Serial.println("Testing connection...");
  if (conn.connect(server_addr, 3306, user, password)) {
    delay(1000);
    Serial.println("Success!");
    conn.close();
  } else {
    Serial.println("ERROR: Connection failed.");
  }
}

void loop() {
  int value;

  // Read the sensor
  value = analogRead(sensorPin);

  // Attempt to connect to the database server
  if (!conn.connected()) {
    if (conn.connect(server_addr, 3306, user, password)) {
      delay(1000);
      dump_cache();  // read any rows stored in the cache file
    }
  }
  // Save the data to the database if connected
  if (conn.connected()) {
    Serial.println("Writing data");
    write_data(value, "NULL");
  } else {
    // Database unreachable, save data to the cache file
    cache_data(value);
  }
  conn.close();
  delay(2000);  // wait for it...
}

