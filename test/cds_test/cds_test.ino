int cds_pin = A4;
void setup() {  
//  analogReference(INTERNAL);
  Serial.begin(9600);
  pinMode(cds_pin,INPUT);
}

void loop() {
  delay(100);
  Serial.println(analogRead(cds_pin));
}
