#define outpin1 2
#define outpin2 3
#define outpin3 4
#define outpin4 5
#define judgepin 12

void setup() {
  pinMode(outpin1, OUTPUT);
  pinMode(outpin2, OUTPUT);
  pinMode(outpin3, OUTPUT);
  pinMode(outpin4, OUTPUT);
  pinMode(judgepin, INPUT_PULLUP);
}

void loop() {
  while(digitalRead(judgepin) == 0){
    delay(100);
  }
  delay(10000);
  digitalWrite(outpin1, HIGH);
  delay(5000);
  digitalWrite(outpin1, LOW);
  delay(5000);
  digitalWrite(outpin2, HIGH);
  delay(5000);
  digitalWrite(outpin2, LOW);
  delay(5000);
  digitalWrite(outpin3, HIGH);
  delay(5000);
  digitalWrite(outpin3, LOW);
  delay(5000);
  digitalWrite(outpin4, HIGH);
  delay(5000);
  digitalWrite(outpin4, LOW);
  delay(5000);
  while(1){
    delay(1000);
  }
}
