#include <EEPROM.h>

int a = 0;
int value1;
int value2;

void setup() {
  Serial.begin(9600);
}

void loop() {
  while (a != 1024){
    value1 = EEPROM.read(a);
    value2 = EEPROM.read(a+1);
    Serial.print(a/2);
    Serial.print("\t");
    Serial.print(value1);
    Serial.print("\t");
    Serial.print(value2);
    Serial.println();
    a = a + 2;
    delay(1);
  }
  while(1) {
    delay(1000000);
  }
}
