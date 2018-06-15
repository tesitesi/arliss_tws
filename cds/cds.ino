int cdsPin = 0;
int pressurePin = 0;
int nichromePin = 0;  //cdsピン番号
int startvalue = 0;
int pressure_value = 0; //パラシュート展開判定用cds返り値
int val1;
int val2;

void setup() {
  pinMode(cdsPin, INPUT);
  pinMode(pressurePin, INPUT);
  pinMode(nichromePin, OUTPUT);
}

void loop() {
  val1 = analogRead(cdsPin);
  if (val1 > startvalue) {
    while(1) {
      val2 = analogRead(pressurePin);
      if (val2 > pressure_value) {
        digitalWrite(nichromePin, HIGH);
        break;
      }
    }
    while(1) {}
  }
  else {}
}
