#include <string.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#define SIZE 15

//float latitude=0;
//float longtitude=0;
//float alt=0; 
TinyGPSPlus gps;
SoftwareSerial mySerial(5, 6); // RX, TX
//TinyGPSCustom magneticVariation(gps, "GPRMC", 10);
 
void setup() {
 // Open serial communications and wait for port to open:
 Serial.begin(115200);
 while (!Serial) {
 ; // wait for serial port to connect. Needed for native USB port only
 }
 
 Serial.println("Goodnight moon!");
 
 // set the data rate for the SoftwareSerial port
 mySerial.begin(9600);
 mySerial.println("Hello, world?");
}
 
void loop() { // run over and over
  while (mySerial.available() > 0){
    char c = mySerial.read();
    //Serial.print(c);
    gps.encode(c);
    Serial.println(strlen(c));
    
    /*
    int  i = 0;
    char *argv[SIZE];
  
    argv[i] = strtok(c, ",");
    do {
       argv[++i] = strtok(NULL, " ");
     } while ( (i < SIZE) && (argv[i] != NULL));
     Serial.println(argv[6]);
  
   if (argv[6]==0) {
     Serial.println("gps no fix");
    } 
    

  else {
  */
    if (gps.location.isUpdated()){
     Serial.print("LAT="); Serial.println(gps.location.lat(), 6);
     Serial.print("LONG="); Serial.println(gps.location.lng(), 6);
     Serial.print("ALT="); Serial.println(gps.altitude.meters()); 
  // }     
  }
 }
}
