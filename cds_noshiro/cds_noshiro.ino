#include <EEPROM.h>
int cdsPin=A6;

void setup() {
  Serial.begin(9600);
  pinMode(cdsPin,INPUT);
}


void loop() {
  for (int i =0; i<1024; i++){
    Serial.print(analogRead(cdsPin));
    Serial.println("");  
  }
}
