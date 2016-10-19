/**
 * Example: Hello, MySQL!
 *
 * This code module demonstrates how to create a simple database-enabled
 * sketch.
 */
#include <Ethernet.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

/* Setup for Ethernet Library */
byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress server_addr(10, 0, 1, 23);   // The IP address of your database server 

/* Setup for the Connector/Arduino */
EthernetClient client;
MySQL_Connection conn((Client *)&client);

char user[] = "root";
char password[] = "secret";
char INSERT_SQL[] = "INSERT INTO test_arduino.hello VALUES ('Hello from Arduino!', NULL)";

void setup() {  
  Ethernet.begin(mac_addr);
  Serial.begin(115200);
  while (!Serial); 
  Serial.println("Connecting...");
  if (conn.connect(server_addr, 3306, user, password))
  {
    delay(500);
    /* Write Hello, World to MySQL table test_arduino.hello */
    // Initiate the query class instance
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    // Execute the query
    cur_mem->execute(INSERT_SQL);
    delete cur_mem;
    Serial.println("Query Success!");
  }
  else
    Serial.println("Connection failed.");
}

void loop() {  
}

