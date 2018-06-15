int cds_Pin = 0;
int pressure_Pin = 0;
int nichrome_Pin = 0;  //cdsピン番号
int brightness = 0;
int pressure_value = 0; //パラシュート展開判定用cds返り値
int var = 0;
int cds;
unsigned long timer;
int pressure;
int cnt1;
int cnt2;

void setup() {
  pinMode(cds_Pin, INPUT);
  pinMode(pressure_Pin, INPUT);
  pinMode(nichrome_Pin, OUTPUT);
}

void loop(){
  switch (var) {
    case 0:
      cnt1 = 0;
      cds = analogRead(cds_Pin);
      while (cnt1 < 10){
        if (cds > brightness){
          cnt1 ++;
        } else {
          cnt1 = 0;
        }
      }
      var = 1;
      timer = millis();
    case 1:
      cnt2 = 0;
      pressure = analogRead(pressure_Pin);
      while (cnt2 < 10 && (millis() - timer) < 100000){
        if (pressure > pressure_value){
          cnt2 ++;
        } else {
          cnt2 = 0;
        }
        var = 2;
      }
    case 2:
      digitalWrite(nichrome_Pin, HIGH);
  }
}
