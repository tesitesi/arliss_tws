int outpin = 13;

void setup() {
  Serial.begin(9600);
  pinMode(outpin,OUTPUT);
}

void loop() {
  Serial.write("wait...");
  if (Serial.read() != -1){
    Serial.write("start min...");
    for (int i=0; i<500; i++){
      propo(0);
    }
    Serial.write("start max...");
    for (int i=0; i<300; i++){
      propo(1000);
    }
    Serial.write("start min...");
    for (int i=0; i<300; i++){
      propo(0);
    }
    Serial.write("start...");
    for (int i=0; i<200; i++){
      propo(150);
    }
    for (int i=0; i<200; i++){
      propo(250);
    }
    while (true){
      propo(0);
    }
  }
  for (int i=0; i<50; i++){
    propo(0);
  }
}


//0-1000で指定
void propo (int value){
  digitalWrite(outpin,HIGH);
  delayMicroseconds(1000 + value);
  digitalWrite(outpin,LOW);
  delay(19 - value / 1000);
}
