#define outpin 13
#define judgepin 2
int var = 0;
int i = 0;

void setup() {
  Serial.begin(9600);
  pinMode(outpin,OUTPUT);
  pinMode(judgepin,INPUT_PULLUP);
}

void loop() {
  int ch[8] = {500,500,0,500,500,500,500,0};
  switch (var) {
      case 0:
      while (digitalRead(judgepin) == LOW) {
        PPM(ch);
      }
      var ++;
    case 1:
      ch[1] = 1000; //elevator
      ch[7] = 500; //fight mode
      while (i<150) {
       PPM(ch); 
       i++;
      }
      var ++;
      break;
    case 2:
      ch[7] = 1000; //flight mode 
      while(1){
        PPM(ch);
     }
  }
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
