#include <EEPROM.h>

int a = 0;
int value;

void setup() {
  Serial.begin(9600);
}

void loop() {
  while (a != 1024){
    value = EEPROM.read(a);
    Serial.print(a);
    Serial.print("\t");
    Serial.print(value);
    Serial.println();
    a = a + 1;
    delay(1);
  }
  while(1) {
    delay(1000000);
  }
}
