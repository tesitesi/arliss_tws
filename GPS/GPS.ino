#include <string.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

//float latitude=0;
//float longtitude=0;
//float alt=0; 
TinyGPSPlus gps;
SoftwareSerial mySerial(2, 3); // RX, TX

unsigned long time;
boolean SW=false;
 
void setup() {
 // Open serial communications and wait for port to open:
 Serial.begin(115200);
 while (!Serial) {
 ; // wait for serial port to connect. Needed for native USB port only
 }
 
 //Serial.println("Goodnight moon!");
 
 // set the data rate for the SoftwareSerial port
 mySerial.begin(9600);
 //mySerial.println("Hello, world?");
}

void loop() {
  Serial.println("A");
  GPS();
  delay(1000);
}

void GPS() {
  unsigned long k = millis();
  
  SW = false;
  while (SW == false) {
  //Serial.println(mySerial.available());
  while (mySerial.available() > 0){
    char c = mySerial.read();
    gps.encode(c);

    if (gps.location.isUpdated()){
     Serial.print("LAT="); Serial.println(gps.location.lat(), 6);
     Serial.print("LONG="); Serial.println(gps.location.lng(), 6);
     Serial.print("ALT="); Serial.println(gps.altitude.meters()); 
     SW = true;  
     } 
     //Serial.println(millis()-k);
     if (millis()-k>1000) {
      SW = true;
      }
    }  
  }  
}
