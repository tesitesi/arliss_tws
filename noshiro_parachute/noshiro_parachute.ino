#include <EEPROM.h>

int nichrompin1=2;
int nichrompin2=3;
int nichrompin3=4;
int nichrompin4=5;
int cdsPin=A6;
int pressurePin=A4;
int wait_time=20;
int case_time=20;
int parachute_time=40; //wait+parachuteで３本目溶断
int var=0;
int cds;
int brightness=200*4;
int cnt;
unsigned long timer;

void setup() {
  Serial.begin(9600);  
  pinMode(pressurePin,INPUT);
  pinMode(nichrompin1,OUTPUT);
  pinMode(nichrompin2,OUTPUT);
  pinMode(nichrompin3,OUTPUT);
  pinMode(nichrompin4,OUTPUT);
}

void loop() {
  switch(var){
    case 0:
      cnt=0;
      while(cnt<2){
        cds=analogRead(cdsPin);
        if (cds>brightness){
          cnt ++;
          delay(1000);
        }else{
          cnt=0;
          Serial.print(cds);
        }
      }
      var=1;
    case 1:
      for (int i = 0; i < 1024; i+=2) {
        EEPROM.write(i, analogRead(cdsPin)/4);
        EEPROM.write(i+1, analogRead(pressurePin));
        if (i==0) {
          digitalWrite(nichrompin1,HIGH);    
        }
        else if (i==6) {
          digitalWrite(nichrompin1,LOW);
          digitalWrite(nichrompin3,HIGH);       
        }
        else if (i==12) {
         digitalWrite(nichrompin3,LOW);    
        }
        delay(1000);
      }
      var=2;
    case 2:
      while (1) {
        delay(1000);
      }
  }    
}
