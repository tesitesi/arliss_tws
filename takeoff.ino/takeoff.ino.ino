#define outpin A7
int var = 0;
int i = 0;

void setup() {
  Serial.begin(9600);
  pinMode(outpin,OUTPUT);
}

void loop() {
  int ch[8];
  ch[0] = 500; 
  ch[1] = 1000; //elevator
  ch[2] = 0;   //throttle
  ch[3] = 500; //airlon
  ch[4] = 500; 
  ch[5] = 500; 
  ch[6] = 500; 
  switch (var) {
    case 0:
      ch[7] = 0; //fight mode
      while (i<3000) {
       PPM(ch); 
       i++;
      }
      var = 1;
      break;
    case 1:
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
