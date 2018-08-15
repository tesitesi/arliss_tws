#define outpin 13
#define signalpin 2
int var=0;
int i=0;
int k=0;
int wait_time=2;
int arming_time=3;
int nose_up_time=2;
int throttol_time=0;
int ch[8]={500,900,0,500,500,500,500,0};

void setup() {
  Serial.begin(9600);
  pinMode(outpin,OUTPUT);
  pinMode(signalpin,INPUT_PULLUP);
}

void loop() {
  switch (var) {
      case 0:
       Serial.println("case0 wait");
       while (digitalRead(signalpin) == LOW) {
          Serial.println("loop");
          PPM(ch);
        }
        while (i<50*wait_time) {
          PPM(ch);
          i++;
        }
        i = 0;
        var++;
        break;
       
      /*ARMING*/
      case 1:
        Serial.println("case1 arm");
        ch[3]=1100;//rudder
        while (i<50*arming_time) {
          PPM(ch);
          i++;
        }
        i = 0;
        var++;
        break;
      case 2:
        Serial.println("case2 elevator only");
        ch[1]=1100; //elevator
        ch[3]=500;
        ch[7]=1000; //fight mode==auto
        while (i<50*nose_up_time) {
        PPM(ch); 
        i++;
        }
        i = 0;
        var ++;
        break;
      case 3:
        Serial.println("case3 elevator&throttle");
        ch[2]=300; //throttol
        while (i<50*throttol_time) {
          PPM(ch);
          i++;
        }
        i = 0;
        var ++;
        break;
      case 4:
        Serial.println("case4 fly");
        ch[1]=100; //elevator
        ch[2]=0; //throttol
        ch[7]=1000;
        while(1){
          PPM(ch);
          i ++;
        }
        break;
  }
}

void PPM(int ch[8]){
  for (int n=0; n<=7; n++){
    onepulth(ch[n]);
  }
  int sum = 0;
  for (int n=0; n<=7; n++){
    sum += ch[n];
  }
  onepulth(12000 - sum);//20000-8000(最小信号)
}

void onepulth(int value){
  digitalWrite(outpin,HIGH);
  delayMicroseconds(250);
  digitalWrite(outpin,LOW);
  delayMicroseconds(750+value);
}
