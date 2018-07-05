#include <EEPROM.h>

int nichrompin1 = 0;
int nichrompin2 = 0;
int nichrompin3 = 0;
int nichrompin4 = 0;
int cdsPin = A6;
int pressurePin = A4;

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
  delay(10000);
   for (int i = 0; i < 1024; i+=2) {
    EEPROM.write(i, analogRead(cdsPin)/4);
    EEPROM.write(i+1, analogRead(pressurePin)/4);
    if (i == 0) {
      digitalWrite(nichrompin1,HIGH);
      digitalWrite(nichrompin2,HIGH);
    }
    else if (i == 5) {
        digitalWrite(nichrompin1,LOW);
        digitalWrite(nichrompin2,LOW);
        digitalWrite(nichrompin3,HIGH);
        digitalWrite(nichrompin4,HIGH);
    }
    else if (i == 10) {
        digitalWrite(nichrompin3,LOW);  
        digitalWrite(nichrompin4,LOW);
    }
    delay(1000);
  }
}
