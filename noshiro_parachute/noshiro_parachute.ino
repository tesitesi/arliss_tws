#include <EEPROM.h>

int nichrompin1=2;
int nichrompin2=3;
int nichrompin3=4;
int nichrompin4=5;
int signalpin=7;
int cdsPin=A6;
int pressurePin=A4;
int first_wait=10;
int wait_time=20;
int case_time=20;
int parachute_time=40; //wait+parachuteで３本目溶断
int k=0;
int var=0;
int cds;
int brightness=700;
int cnt;
unsigned long timer;

void setup() {
  Serial.begin(9600);  
  pinMode(pressurePin,INPUT);
  pinMode(nichrompin1,OUTPUT);
  pinMode(nichrompin2,OUTPUT);
  pinMode(nichrompin3,OUTPUT);
  pinMode(nichrompin4,OUTPUT);
  pinMode(signalpin,OUTPUT);
  digitalWrite(signalpin,LOW);
}

void loop() {
  while (k<first_wait) {
    delay(1000);
    k++;
  }
  switch(var){
    case 0: // start--injection
      cnt=0;
      while(cnt<2){
        cds=analogRead(cdsPin);
        if (cds>brightness){
          cnt ++;
          delay(500);
          cnt=0;
          Serial.print(cds);
        }
      }
      var=1;
      digitalWrite(signalpin,HIGH);
      break;
    case 1: // tegusu-cut
      for (int i = 0; i < 1024; i+=2) {
        EEPROM.write(i, analogRead(cdsPin)/4);
        EEPROM.write(i+1, analogRead(pressurePin));
        if (i==0) {
          digitalWrite(nichrompin1,HIGH);    
        }
        else if (i==6) {
          digitalWrite(nichrompin1,LOW);
        }
        else if (i==8) {
          digitalWrite(nichrompin3,HIGH);       
        }
        else if (i==14) {
         digitalWrite(nichrompin3,LOW);    
        }
        delay(1000);
      }
      var=2;
      break;
    case 2: // end, just wait
      while (1) {
        delay(1000);
      }
      break;
  }    
}
