int cdsPin = XX;
int pressurePin = YY;
int nichromePin = ZZ;  //cdsピン番号
int strartvalue = XX;
int pressure_value = YY; //パラシュート展開判定用cds返り値
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
    void while(1) {
      val2 = analogRead(pressurePin);
      if (val2 > pressure_value) {
        digitalWrite(nichromePin, HIGH)
        break;
      }
    }
    while(1) {}
  }
  else {}
}
