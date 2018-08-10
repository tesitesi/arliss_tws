#define outpin 13
#define signalpin 2
int var = 0;
int i = 0;
int k = 0;
int nose_up_time = 5;
int throttol_time = 3;

void setup() {
  Serial.begin(9600);
  pinMode(outpin,OUTPUT);
  pinMode(judgepin,INPUT_PULLUP);
}

void loop() {
  int ch[8] = {500,500,0,500,500,500,500,0};
  switch (var) {
      case 0:
      while (digitalRead(signalpin) == LOW) {
        PPM(ch);
      }
      delay(2000);
      var ++;
    case 1:
      ch[1] = 0; //elevator
      ch[7] = 1000; //fight mode
      while (i<50*nose_up_time) {
       PPM(ch); 
       i++;
      }
      var ++;
      break;
    case 2:
      ch[1] = 0;
      ch[2] = 300; //throttol
      while (k<50*throttol_time) {
        PPM(ch);
        k++;
      }
      var ++;
      break;
    case 3:
      ch[1] = 500; //elevator
      ch[2] = 0; //throttol
      ch[7] = 1000; //flight mode 
      while(1){
        PPM(ch);
        k ++;
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
