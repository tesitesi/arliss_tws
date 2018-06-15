int cds_Pin = 0;
int pressure_Pin = 0;
int nichrome_Pin = 0;  //cdsピン番号
int brightness = 0;
int pressure_value = 0; //パラシュート展開判定用cds返り値

void setup() {
  pinMode(cds_Pin, INPUT);
  pinMode(pressure_Pin, INPUT);
  pinMode(nichrome_Pin, OUTPUT);
}

int var = 0;

void loop(){
  switch (var) {
    case 0:
      int cds = analogRead(cdsPin)
      if (cds > brightness) {
        var = 1;
        unsigned long timer = millis();
      }
    case 1:
      int pressure = analogRead(pressure_Pin);
      if (pressure > pressure_value) {
        var = 2;
      }
      //タイマーの冗長系
      if (millis() - timer > 100000) {
        var = 2;
      }
    case 2:
      digitalWrite(nichrome_Pin, HIGH);
}
