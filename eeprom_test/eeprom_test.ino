#include <EEPROM.h>

int pressurePin = A1;

void setup() {
  pinMode(pressurePin, INPUT);
  for (int i = 0; i < 512; i++) {
    EEPROM.write(i, i);
    delay(500);
  }
}

void loop() {}
