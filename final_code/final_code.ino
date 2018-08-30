/*****************************
 *ARLISS2018 THE WRIGHT STAFF*
 *SOURCE CODE FOR ARDUINO    *
 *****************************/

#include <SPI.h>
#include<Wire.h>
#include "pins_arduino.h"
#include <string.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#define AVE_NUM    20   // 圧力・温度のＡ／Ｄ変換値を平均化する回数
#define H_CORRECT   0    
#define cds_Pin A6
#define nichrompin1 2
#define nichrompin2 3
#define nichrompin3 4
#define nichrompin3 5
#define nichrompin4 6
#define signalpin   7
#define brightness 700 //放出判定のCdS値
#define hight 1000     //放出高度

int now;
float a0, b1, b2, c12;      // 自宅でのセンサと実際の高度差補正値(My自宅の標高は100m)
float h_pre=0;              // 1つ前のGPS高度を格納
boolean ReleaseJg = false;  // ケース放出時にtrue
boolean HightJg = false;    // 2回連続一定高度以下でtrue
boolean HightSW = false;    // 1回一定高度以下でtrue

unsigned long time;
unsigned long Press, Temp;  // 圧力および温度の変換値を保存する変数

TinyGPSPlus gps;
SoftwareSerial mySerial(8 ,9); // RX, TX

void setup() {
  while (!Serial) {
  ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Goodnight moon!");
  mySerial.begin(9600);
  mySerial.println("Hello, world?");

  pinMode(cds_Pin, INPUT);
  Serial.begin(115200);
  // ＳＰＩの初期化
  SPI.begin() ;                        // ＳＰＩを行う為の初期化
  SPI.setBitOrder(MSBFIRST) ;          // ビットオーダー
  SPI.setClockDivider(SPI_CLOCK_DIV4) ;// クロック(CLK)をシステムクロックの1/4で使用(16MHz/4)
  SPI.setDataMode(SPI_MODE0) ;         // クロック極性０(LOW)　クロック位相１(HIGH)

  pinMode(nichrompin1,OUTPUT);
  pinMode(nichrompin2,OUTPUT);
  pinMode(nichrompin3,OUTPUT);
  pinMode(nichrompin4,OUTPUT);

  delay(3000) ;                        // 3Sしたら開始
}

void loop() {
  if (ReleaseJg = false) {
    cdsJg();
  }
  Hight_Judge();
  Time();
  GPS();
  if (ReleaseJg == true) {
    if (HightJg ==true) {
      NichromCut();
    }
  }
}
/*
 * ケース放出判定
 */
void cdsJg() {
  int i=0;
  while (i<10) {
    if (analogRead(cds_Pin) > brightness) {
      i++;
      GPS();
    }
    else{
      i=0;
      GPS();
    }
    ReleaseJg = true;
  }
  now = millis()-10000; //放出時の時間を記録
}

float alt_() {
   int i;
   float ans;
   unsigned long p, t;

   p=t=0;
   for (i=0 ; i<AVE_NUM ; i++) {    // ２０回読み込んで平均化する
      PressureRead();                // メモリーマップから圧力および温度のＡ／Ｄ変換値を読み出す
      p = p+Press;
      t = t+Temp;
   }
   Press = p/AVE_NUM;
   Temp  = t/AVE_NUM;

   // ans = PressureCalc();            // 気圧値の計算を行う
   //Serial.print(ans);                // 気圧値の表示を行う
   //Serial.print(" hPa ");
   ans=AltitudeCalc(ans,H_CORRECT);  // 高度の計算を行う
   //Serial.print(ans);                // 高度の表示を行う
   //Serial.println(" m");

   //return ans;
   return 1;
}

// メモリーマップから係数を読み出す処理
void CoefficientRead() {
   unsigned int h, l;

   digitalWrite(SS,LOW);               // SS(CS)ラインをLOWにする

   // ａ０の係数を得る
   SPI.transfer(0x88);                 // a0(MSB:HIGH byte)係数
   h = SPI.transfer(0x00);
   SPI.transfer(0x8a);                 // a0(LSB:LOW byte) 係数
   l = SPI.transfer(0x00);
   a0 = (h<<5)+(l>>3)+(l&0x07)/8.0;
   Serial.println(h,l);

   // ｂ１の係数を得る
   SPI.transfer(0x8c);                 // b1(MSB:HIGH byte)係数
   h = SPI.transfer(0x00);
   SPI.transfer(0x8e);                 // b1(LSB:LOW byte) 係数
   l = SPI.transfer(0x00);
   b1 = ((((h & 0x1F)*0x100)+l)/8192.0)-3;

   // ｂ２の係数を得る
   SPI.transfer(0x90);                 // b2(MSB:HIGH byte)係数
   h = SPI.transfer(0x00);
   SPI.transfer(0x92);                 // b2(LSB:LOW byte) 係数
   l = SPI.transfer(0x00);
   b2 = ((((h-0x80)<<8)+l)/16384.0)-2;

   // Ｃ１２の係数を得る
   SPI.transfer(0x94);           // c12(MSB:HIGH byte)係数
   h = SPI.transfer(0x00);
   SPI.transfer(0x96);           // c12(LSB:LOW byte) 係数
   l = SPI.transfer(0x00);
   c12 = (((h*0x100)+l)/16777216.0);

   SPI.transfer(0x00);
   digitalWrite(SS,HIGH);        // SS(CS)ラインをHIGHにする
}

// メモリーマップから圧力および温度のＡ／Ｄ変換値を読み出す処理
void PressureRead() {
   unsigned int h, l;

   // 圧力および温度の変換を開始させる
   digitalWrite(SS,LOW);         // SS(CS)ラインをLOWにする
   SPI.transfer(0x24);           // 0x24コマンドの発行(圧力と温度の変換)
   SPI.transfer(0x00);
   digitalWrite(SS,HIGH);        // SS(CS)ラインをHIGHにする
   delay(3);                     // 変換完了まで３ｍｓ待つ

   digitalWrite(SS,LOW);         // SS(CS)ラインをLOWにする

   // 圧力のＡ／Ｄ変換値を得る
   SPI.transfer(0x80);           // 圧力(MSB:HIGH byte)
   h = SPI.transfer(0x00);
   SPI.transfer(0x82);           // 圧力(LSB:LOW byte)
   l = SPI.transfer(0x00);
   Press=((h*256)+l)/64;

   // 温度のＡ／Ｄ変換値を得る
   digitalWrite(SS,LOW);         // SS(CS)ラインをLOWにする
   SPI.transfer(0x84);           // 温度(MSB:HIGH byte)
   h = SPI.transfer(0x00);
   SPI.transfer(0x86);           // 温度(LSB:LOW byte)
   l = SPI.transfer(0x00);
   Temp=((h*256)+l)/64;

   SPI.transfer(0x00);
   digitalWrite(SS,HIGH);        // SS(CS)ラインをHIGHにする
}
// 気圧値(hPa)を計算する処理
float PressureCalc() {
   float ret, f;

   f=a0+(b1+c12*Temp)*Press+b2*Temp;
   ret=f*(650.0/1023.0)+500.0;
   //Serial.println(a0);
   return ret;
}
// 気圧値(hPa)から高度を計算する処理
float AltitudeCalc(float pressure,int Difference)
{
   float h;

   h=44330.8*(1.0-pow((pressure/1013.25), 0.190263));
   h=h+Difference;
   return h;
}

// 高度判定 2回連続一定高度以下でHightJG=true
void Hight_Judge() {
  float h = alt_();
  if (abs(h_pre-h)<100) {     //外れ値の除外
    if (h < hight) {
      if (HightSW = false) {
        HightSW = true;
      }
      else {
        HightJg = true;
      }
    }
    else {
      HightSW = false;
    }
  }
  else {
    HightSW = false;
  }
}

//てぐす溶断
void NichromCut() {
  int case_time = 20;
  digitalWrite(signalpin,HIGH);           //機体側Arduinoへの信号
  for (int i = 0; i < 1024; i++) {
    GPS();
    if (i==0) {
        digitalWrite(nichrompin1,HIGH);
        Serial.println("nichrom1");
      }
      else if (i==3) {
        digitalWrite(nichrompin1,LOW);
        digitalWrite(nichrompin2,HIGH);
      }
      else if (i==6) {
        digitalWrite(nichrompin2,LOW);
      }
      else if (i==7) {
        digitalWrite(nichrompin3,HIGH);  
        Serial.print("nichrom3");     
      }
      else if (i==10) {
        digitalWrite(nichrompin3,LOW);
        digitalWrite(nichrompin4,HIGH);   
      }
      else if (i==13) {
        digitalWrite(nichrompin4,LOW);
      }
      
      delay(1000);
  }  
}

void GPS() {
  while (mySerial.available() > 0){
    char c = mySerial.read();
    //Serial.print(c);
    gps.encode(c);
    Serial.println(strlen(c)); 
    if (gps.location.isUpdated()){
      Serial.print("LAT="); Serial.println(gps.location.lat(), 6);
      Serial.print("LONG="); Serial.println(gps.location.lng(), 6);
      Serial.print("ALT="); Serial.println(gps.altitude.meters()); 
    }
  }     
}

//放出から一定時間で強制的に展開シーケンスへ移行
void Time() {
  if (ReleaseJg==true) {
    if (now-millis()>300*1000) {
      HightJg=true;
    }
  }
}

