/**
  Example Arduino redundant data collector
  
  This project demonstrates how to create a data
  collector master/slave for redundancy. If the master
  fails, the slave will take over.

  The sketch has both the master and slave code with the
  default the slave role. To use the sketch as a master, 
  comment out the #define SLAVE.
*/
#include <Wire.h>

// TODO: Add MySQL Connector/Arduino include, variables here

#define SLAVE

int address = 8;            // Address on I2C bus

// Blink the LED on pin 13
void blink() {
  digitalWrite(13, HIGH);  
  delay(1000);              
  digitalWrite(13, LOW);   
}

// Record data to database
// Stubbed method for writing data
void recordData() {
  // TODO: Complete this code for your sensor read and the write to MySQL. 

#if defined SLAVE
unsigned long startTime;        // start of simple timer 
unsigned long elapsedTime;      // number of milliseconds elapsed
unsigned long maxTime = 10000;  // timeout value (10 seconds)

// Get the heartbeat message from the master
void getHeartbeat(int chars) {
  char ch;
  while (Wire.available()) { 
    ch = Wire.read(); 
  } 
  Serial.println("Master is ok.");
  // Reset timer since master is Ok
  startTime = millis();           
  elapsedTime = 0;    
  blink();  // visual feedback
}

#else

// Send the heartbeat pulse
void sendHeartbeat() {
  Wire.beginTransmission(address); 
  Wire.write("hello!");       
  Wire.endTransmission();    
  blink();  // visual feedback
}

#endif

void setup() {
#if defined SLAVE
  Wire.begin(address);            // join i2c bus 
  Wire.onReceive(getHeartbeat);   // register the heartbeat code
  startTime = millis();           // initiate timer           
  elapsedTime = 0;  
#else
  Wire.begin();
#endif 
  pinMode(13, OUTPUT);          // turn on onboard LED (pin 13)
  Serial.begin(115200);         
  // TODO: Add any database initialization code here
}

void loop() {
#if defined SLAVE
  Serial.println("looping...");
  // Check timeout
  elapsedTime += millis() - startTime;
  Serial.println(elapsedTime);
  if (elapsedTime > maxTime) {
    // Master is dead. Read and save the sensor data!
    Serial.println("Master has died! Oh, my...");
    // OPTIONAL: reset master timer to try again.
    startTime = millis();  
    elapsedTime = 0;

    // Record data since master is dead.
    recordData();   
  }  
#else
  recordData(); // read and save data
  Serial.print("Sending heartbeat...");
  sendHeartbeat();
  Serial.println("done.");
#endif 
  delay(3000);  // wait 3 seconds
}


