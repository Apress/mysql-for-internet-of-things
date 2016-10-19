/**
  Data Aggregator Example
  
  This project demonstrates how to receive sensor data from
  an XBee connected network. It uses an Arduino Mega with an XBee shield
  with an XBee coordinator installed.
  
  Note: This sketch was adapted from the examples in the XBee 
  library created by Andrew Rapp.
*/

#include <XBee.h>
#include <SoftwareSerial.h>
#include <Ethernet.h>
#include <SPI.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

// Setup pin definitions for XBee shield
uint8_t recv = 2;
uint8_t trans = 3;

SoftwareSerial soft_serial(recv, trans);

// assign a MAC address and IP address for the Arduino
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress server_addr(10, 0, 1, 24); 

/* Setup for the Connector/Arduino */
EthernetClient client;
MySQL_Connection conn((Client *)&client);

char user[] = "root";
char password[] = "SECRET";

// Instantiate an instance of the XBee library
XBee xbee = XBee();

// Instantiate an instance of the IO sample class
ZBRxIoSampleResponse ioSample = ZBRxIoSampleResponse();

String get_data(ZBRxIoSampleResponse *ioSample) {  
  // Received data from address of data node
  int address = (ioSample->getRemoteAddress64().getMsb() << 8) +  
                 ioSample->getRemoteAddress64().getLsb();  

  // Get and calculate the temperature in C and F
  float temp_raw = ioSample->getAnalog(3);
  float temp_c = ((temp_raw * 1200.0 / 1024.0) - 500.0) / 10.0;
  float temp_f = ((temp_c * 9.0)/5.0) + 32.0;
  String strRow = String("(");
  strRow += String(address);
  strRow += ",";
  strRow += String(temp_raw);
  strRow += ",";
  strRow += String(temp_c);
  strRow += ",";
  strRow += String(temp_f);
  strRow += ")";
  
  return strRow;
}

void setup() { 
  // start the SPI library:
  SPI.begin();

  // start the Ethernet connection and the server:
  Ethernet.begin(mac);
  Serial.begin(9600);
  while (!Serial);
  soft_serial.begin(9600);
  xbee.setSerial(soft_serial);
  Serial.println("Connecting to MySQL...");
  if (conn.connect(server_addr, 3306, user, password))
  {
    delay(500);
    Serial.println("Success!");
  }
  else
  {
    Serial.println("Connection failed.");
  }
}

void loop() {
  String row[3];
  int i;
  
  i = 0;
  //attempt to read a packet    
  xbee.readPacket();
  if (xbee.getResponse().isAvailable()) {
    // XBee module is communicating, check for IO packet
    if (xbee.getResponse().getApiId() == ZB_IO_SAMPLE_RESPONSE) {
      // Get the packet 
      xbee.getResponse().getZBRxIoSampleResponse(ioSample);
      row[i] = get_data(&ioSample);
      i++;
    } 
    else {
      Serial.print("Expected I/O Sample, but got ");
      Serial.print(xbee.getResponse().getApiId(), HEX);
    }    
  } else if (xbee.getResponse().isError()) {
    Serial.print("Error reading packet.  Error code: ");  
    Serial.println(xbee.getResponse().getErrorCode());
  }
  // Store the data once 3 entries are retrieved.
  if (i == 3) {
    i = 0;
    String strINSERT = String("INSERT INTO test.room_temperatures VALUES ");
    strINSERT += row[0];
    strINSERT += ",";
    strINSERT += row[1];
    strINSERT += ",";
    strINSERT += row[2];
    Serial.println(strINSERT);    
    // Create an instance of the cursor passing in the connection
    MySQL_Cursor *cur = new MySQL_Cursor(&conn);
    cur->execute(strINSERT.c_str());
    delete cur;
  }
}
