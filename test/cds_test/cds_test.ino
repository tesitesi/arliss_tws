int cds_pin = A6;
void setup() {
  Serial.begin(9600);
  pinMode(cds_pin,INPUT);
}

void loop() {
  delay(100);
  Serial.println(analogRead(cds_pin));
}
