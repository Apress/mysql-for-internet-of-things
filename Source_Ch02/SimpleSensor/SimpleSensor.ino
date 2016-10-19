/*
  Simple Sensor - MySQL for the IOT
  
  For this sketch, we explore a simple sensor (a pushbutton) and a simple
  response to sensor input (a LED). When the sensor is activated (the 
  button is pushed), the LED is illuinated.
*/
 
int led = 13;     // LED on pin 13
int button = 2;   // button on pin 2

// the setup routine runs once when you press reset:
void setup() {                
  // initialize pin 13 as an output.
  pinMode(led, OUTPUT);
  pinMode(button, INPUT);  
}

// the loop routine runs over and over again forever:
void loop() {
  // read the state of the sensor
  int state = digitalRead(button);

  // if sensor engaged (button is pressed), turn on LED
  if (state == HIGH) {    
    digitalWrite(led, HIGH);
  }
  // else turn off LED
  else {
    digitalWrite(led, LOW);
  }
}
