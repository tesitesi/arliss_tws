#include <EEPROM.h>

int cdsPin = A6;
int pressurePin = A4;

void setup() {
  Serial.begin(9600);
  pinMode(cdsPin, INPUT);
  pinMode(pressurePin, INPUT);
  }

void loop() {
 // for (int i = 0; i < 100; i++) {
   // delay(1000);
  //}
  for (int i = 0; i < 1024; i+=2) {
    EEPROM.write(i, analogRead(cdsPin)/4);
    EEPROM.write(i+1, analogRead(pressurePin)/4);
    delay(1000);
  }
  while(1){
    delay(10000);
  }
}
