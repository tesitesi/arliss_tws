#include <SPI.h>
#include "pins_arduino.h"
float a0 , b1 , b2 , c12 ;
#define AVE_NUM      20                   // 圧力・温度のＡ／Ｄ変換値を平均化する回数
#define H_CORRECT   0                    // 自宅でのセンサと実際の高度差補正値(My自宅の標高は100m)

unsigned long Press , Temp ;              // 圧力および温度の変換値を保存する変数

void setup()
{
     // シリアルモニターの設定
     Serial.begin(115200) ;
     // ＳＰＩの初期化
     SPI.begin() ;                        // ＳＰＩを行う為の初期化
     SPI.setBitOrder(MSBFIRST) ;          // ビットオーダー
     SPI.setClockDivider(SPI_CLOCK_DIV4) ;// クロック(CLK)をシステムクロックの1/4で使用(16MHz/4)
     SPI.setDataMode(SPI_MODE0) ;         // クロック極性０(LOW)　クロック位相１(HIGH)

     delay(3000) ;                        // 3Sしたら開始
     CoefficientRead() ;                  // メモリーマップから係数を読み出して置く
}
void loop() {
 GPS(); 
}
float GPS()
{
     int i ;
     float ans ;
     unsigned long p , t ;

     p = t = 0 ;
     for (i=0 ; i < AVE_NUM ; i++) {      // ２０回読み込んで平均化する
          PressureRead() ;                // メモリーマップから圧力および温度のＡ／Ｄ変換値を読み出す
          p = p + Press ;
          t = t + Temp ;
     }
     Press = p / AVE_NUM ;
     Temp  = t / AVE_NUM ;

     ans = PressureCalc() ;               // 気圧値の計算を行う
     Serial.print(ans) ;                  // 気圧値の表示を行う
     Serial.print(" hPa    ") ;
     ans = AltitudeCalc(ans,H_CORRECT) ;  // 高度の計算を行う
     Serial.print(ans) ;                  // 高度の表示を行う
     Serial.println(" m") ;

     delay(1000) ;                        // １秒後に繰り返す
     return ans;
}
// メモリーマップから係数を読み出す処理
void CoefficientRead()
{
     unsigned int h , l ;

     digitalWrite(SS,LOW) ;               // SS(CS)ラインをLOWにする

     // ａ０の係数を得る
     SPI.transfer(0x88) ;                 // a0(MSB:HIGH byte)係数
     h = SPI.transfer(0x00) ;
     SPI.transfer(0x8a) ;                 // a0(LSB:LOW byte) 係数
     l = SPI.transfer(0x00) ;
     a0 = (h << 5) + (l >> 3) + (l & 0x07) / 8.0 ;
     Serial.println(h,l);

     // ｂ１の係数を得る
     SPI.transfer(0x8c) ;                 // b1(MSB:HIGH byte)係数
     h = SPI.transfer(0x00) ;
     SPI.transfer(0x8e) ;                 // b1(LSB:LOW byte) 係数
     l = SPI.transfer(0x00) ;
     b1 = ( ( ( (h & 0x1F) * 0x100 ) + l ) / 8192.0 ) - 3 ;

     // ｂ２の係数を得る
     SPI.transfer(0x90) ;                 // b2(MSB:HIGH byte)係数
     h = SPI.transfer(0x00) ;
     SPI.transfer(0x92) ;                 // b2(LSB:LOW byte) 係数
     l = SPI.transfer(0x00) ;
     b2 = ( ( ( ( h - 0x80) << 8 ) + l ) / 16384.0 ) - 2 ;

     // Ｃ１２の係数を得る
     SPI.transfer(0x94) ;                 // c12(MSB:HIGH byte)係数
     h = SPI.transfer(0x00) ;
     SPI.transfer(0x96) ;                 // c12(LSB:LOW byte) 係数
     l = SPI.transfer(0x00) ;
     c12 = ( ( ( h * 0x100 ) + l ) / 16777216.0 )  ;

     SPI.transfer(0x00) ;
     digitalWrite(SS,HIGH) ;              // SS(CS)ラインをHIGHにする
}
// メモリーマップから圧力および温度のＡ／Ｄ変換値を読み出す処理
void PressureRead()
{
     unsigned int h , l ;

     // 圧力および温度の変換を開始させる
     digitalWrite(SS,LOW) ;               // SS(CS)ラインをLOWにする
     SPI.transfer(0x24) ;                 // 0x24コマンドの発行(圧力と温度の変換)
     SPI.transfer(0x00) ;
     digitalWrite(SS,HIGH) ;              // SS(CS)ラインをHIGHにする
     delay(3) ;                           // 変換完了まで３ｍｓ待つ

     digitalWrite(SS,LOW) ;               // SS(CS)ラインをLOWにする

     // 圧力のＡ／Ｄ変換値を得る
     SPI.transfer(0x80) ;                 // 圧力(MSB:HIGH byte)
     h = SPI.transfer(0x00) ;
     SPI.transfer(0x82) ;                 // 圧力(LSB:LOW byte)
     l = SPI.transfer(0x00) ;
     Press = ( ( h * 256 ) + l ) / 64 ;

     // 温度のＡ／Ｄ変換値を得る
     digitalWrite(SS,LOW) ;               // SS(CS)ラインをLOWにする
     SPI.transfer(0x84) ;                 // 温度(MSB:HIGH byte)
     h = SPI.transfer(0x00) ;
     SPI.transfer(0x86) ;                 // 温度(LSB:LOW byte)
     l = SPI.transfer(0x00) ;
     Temp = ( ( h * 256 ) + l ) / 64 ;

     SPI.transfer(0x00) ;
     digitalWrite(SS,HIGH) ;              // SS(CS)ラインをHIGHにする
}
// 気圧値(hPa)を計算する処理
float PressureCalc()
{
     float ret , f ;

     f = a0 + ( b1 + c12 * Temp ) * Press + b2 * Temp ;
     ret = f * ( 650.0 / 1023.0 ) + 500.0 ;
     //Serial.println(a0);
     return ret ;
}
// 気圧値(hPa)から高度を計算する処理
float AltitudeCalc(float pressure,int Difference)
{
     float h ;

     h = 44330.8 * (1.0 - pow( (pressure/1013.25) ,  0.190263 )) ;
     h = h + Difference ;
     return h ;
}
