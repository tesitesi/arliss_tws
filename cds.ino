int cdsPin = XX;  //cdsピン番号
int strartvalue = XX; //パラシュート展開判定用cds返り値
int val;

void setup() {
  pinMode(cdsPin, INPUT);
}

void loop() {
  val = analogRead(cdsPin);
  if (val > startvalue) {
    //パラシュート展開から切り離しまで
    break;
  }
  else {}
}
