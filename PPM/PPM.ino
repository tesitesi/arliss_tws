#define outpin 13
#define vol1 A0
#define vol2 A1
#define vol3 A2
#define vol4 A3
#define vol5 A4
#define vol6 A5
#define vol7 10
#define vol8 8

void setup() {
  Serial.begin(9600);
  pinMode(outpin,OUTPUT);
}

void loop() {
  int ch[8];
  ch[0] = analogRead(vol1);
  ch[1] = analogRead(vol2);
  ch[2] = analogRead(vol3);
  ch[3] = analogRead(vol4);
  ch[4] = analogRead(vol5);
  ch[5] = analogRead(vol6);
  ch[6] = analogRead(vol7);
  ch[7] = analogRead(vol8);
  PPM(ch);
}

void PPM(int ch[8]){
  for (int i=0; i<=7; i++){
    onepulth(ch[i]);
  }
  int sum = 0;
  for (int i=0; i<=7; i++){
    sum += ch[i];
  }
  onepulth(12000 - sum);//20000-8000(最小信号)
}

void onepulth(int value){
  digitalWrite(outpin,HIGH);
  delayMicroseconds(250);
  digitalWrite(outpin,LOW);
  delayMicroseconds(750+value);
}
