#include <EEPROM.h>
#define outpin 13
#define Reset 12
#define signalpin 7
#define XbeeSW 6
int var;
int i=0;
int k=0;
int wait_time=17;
int arming_time=3;
int nose_up_time=7;
int throttol_time=0;
int ch[8]={500,900,0,500,500,500,500,0};
unsigned long time;
unsigned long Time=0;

void setup() {
  Serial.begin(115200);
  var = EEPROM.read(0);
  Serial.println(var);
  pinMode(outpin,OUTPUT);
  pinMode(XbeeSW,OUTPUT);
  pinMode(signalpin,INPUT_PULLUP);
  pinMode(Reset,INPUT_PULLUP);
}

void loop() {
  switch (var) {
      case 0:
      EEPROM.write(0,0);
       Serial.println("case0 wait");
       while (digitalRead(signalpin) == HIGH) {
          Serial.println("loop");
          PPM(ch);
        }
        Serial.println("放出");
        digitalWrite(XbeeSW,HIGH);  //ケースからの信号でXbeeの通信開始
        while (digitalRead(signalpin) == LOW) {
          Serial.println("loop2");
          PPM(ch);
        }
        Serial.println("一定高度到達");
        while (i<50*wait_time) {
          PPM(ch);
          i++;
        }
        i = 0;
        var++;
        break;
       
      /*ARMING*/
      case 1:
        EEPROM.write(0,1);
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
        EEPROM.write(0,2);
        Serial.println("case2 elevator only");
        ch[1]=900; //elevator
        ch[3]=500;
        ch[7]=500; //fight mode==stabilize
        Serial.println("auto");
        while (i<50*nose_up_time) {
        PPM(ch); 
        i++;
        }
        //i=0;
        var ++;
        break;
      case 3:
        EEPROM.write(0,3);
        Serial.println("case3 fly");
        ch[1]=500; //elevator
        ch[2]=0; //throttol
        ch[7]=1000;
        Time=millis();
        while (digitalRead(4)==LOW) {
          EEPROM.write(0,0);
        }
        while(1){
          PPM(ch);
          i ++;
          unsigned long now=millis(); 
          if ((now-Time)/1000>60) {
            ch[7]=0;//mode==manual
            Serial.println("Manual");
          }
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
