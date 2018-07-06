#include <EEPROM.h>

int nichrompin1 = 2;
int nichrompin2 = 3;
int nichrompin3 = 4;
int nichrompin4 = 5;
int cdsPin = A6;
int pressurePin = A4;
int wait_time = 300;
int case_time = 120;
int parachute_time = 180; //wait+parachuteで３本目溶断

void setup() {
  Serial.begin(9600);
  pinMode(cdsPin,INPUT);
  pinMode(pressurePin,INPUT);
  pinMode(nichrompin1,OUTPUT);
  pinMode(nichrompin2,OUTPUT);
  pinMode(nichrompin3,OUTPUT);
  pinMode(nichrompin4,OUTPUT);
}

void loop() {
  for (int k=0; k < wait_time; k++) {
    delay(1000); 
  }
  for (int i = 0; i < 1024; i+=2) {
    EEPROM.write(i, analogRead(cdsPin)/4);
    EEPROM.write(i+1, analogRead(pressurePin));
    if (i == case_time*2 + 0) {
      digitalWrite(nichrompin1,HIGH);    
    }
    else if (i == case_time*2 + 10) {
      digitalWrite(nichrompin1,LOW);
      digitalWrite(nichrompin2,HIGH);       
    }
    else if (i == case_time*2 + 20) {
      digitalWrite(nichrompin2,LOW);    
    }
    else if (i == parachute_time*2 + 0) {
      digitalWrite(nichrompin3,HIGH);
    }
    else if (i == parachute_time*2 + 10) {
      digitalWrite(nichrompin3,LOW); 
      digitalWrite(nichrompin4,HIGH);
    }
    else if (i == parachute_time*2 + 20) {
      digitalWrite(nichrompin4,LOW);
    }
    delay(1000);
  }
  while (1) {
    delay(1000);
  }
}
