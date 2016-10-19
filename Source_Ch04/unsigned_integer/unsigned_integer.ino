void setup() {
  unsigned int uint_val = 65535;
  long long_val = 65535;
  Serial.begin(9600);
  while (!Serial); // Wait until Serial is ready - Leonardo
  Serial.print("1> ");
  Serial.print(uint_val);
  Serial.print(" ");
  Serial.println(long_val);
  Serial.print("2> ");
  uint_val++;
  long_val++;
  Serial.print(uint_val);
  Serial.print(" ");
  Serial.println(long_val);
  Serial.print("3> ");
  uint_val++;
  long_val++;
  Serial.print(uint_val);
  Serial.print(" ");
  Serial.println(long_val);
}

void loop() {
}
