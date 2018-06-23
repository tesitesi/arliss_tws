#include <EEPROM.h>

void setup () {}

void loop() {
    for (int i = 0; i < 1024; i++) {
      EEPROM.write(i,0);
    }
}
