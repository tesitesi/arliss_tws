/*
MS561101BA用スケッチ
MS5607でも使えるはず（たぶん）
I2Cでセンサデータを取得し、シリアル通信でPCに表示
by ina111
2012/07/16
参考 http://www.ina111.org/archives/305
*/

#include <Wire.h>

//気圧計のアドレス
//#define MS5611_ADDR 0x76 //CBR =HIGHの時は0x76,LOWの時は0x77
#define MS5607_ADDR 0x76

//気圧計で使う変数
uint16_t C_[6] ={46372, 43981, 29059, 27842, 31553, 28165}; //初期値
uint32_t D1=0,D2=0;
int32_t dT,TEMP=0;
int64_t OFF,SENS;
int64_t T2,OFF2,SENS2;
int32_t P=0;
int32_t Height;

unsigned long now;                   //現在時間を入れて変換時間を計算
unsigned long lastD1Conv,lastD2Conv; //最後に変換した時間micros()
unsigned long ConvTime = 10000;      //変換時間[マイクロ秒]
boolean SWD1Conv = true, SWD2Conv = true; //D1,D2どっちを処理しているか
boolean SWD1D2 = true;               //trueでD1の処理,falseでD2の処理

void setup(){
  Wire.begin();
  Serial.begin(9600);
  //Serial.begin(115200);
  delay(100);
  readPROM();
}

void loop(){
  getD1();
  getD2();
  getPressTemp(D1,D2);
  getHeight(P);

  Serial.print(millis());Serial.print("\t");
  Serial.print(TEMP);Serial.print("\t");
  Serial.print(P);Serial.print("\t");
  Serial.println(Height);
}

/*
 * func name  : getD1,getD2
 * processing : MS5611の生データD1,D2を取得
 * param      : 
 * summary    : 後述のstartConvとreadADC使用
 *              変換の時間前だと変換、変換後なら読み出しを行う
 *              D1とD2は交互に読み出されるようにスイッチしている
 * return     : 
 */
void getD1(){
  now = micros();
  //SWD1D2==trueだとD1,falseだとD2
  //SWD1Conv==trueだと変換、falseかつ時間経過後は読み出し
  if(SWD1Conv == true && SWD1D2 == true){
    startConv(0x48); //D1のとき0x48,D2のとき0x58,OSRによって変化
    lastD1Conv = micros();
    SWD1Conv = false;
  }else if(now - lastD1Conv >= ConvTime && SWD1D2 == true){
    D1 = readADC();
    SWD1Conv = true;
    SWD1D2 = false;
  }
}

void getD2(){
  now = micros();
  if(SWD2Conv == true && SWD1D2 == false){
    startConv(0x58); //D1のとき0x48,D2のとき0x58,OSRによって変化
    lastD2Conv = micros();
    SWD2Conv = false;
  }else if(now - lastD2Conv >= ConvTime && SWD1D2 == false){
    D2 = readADC();
    SWD2Conv = true;
    SWD1D2 = true;
  }
}

/*
 * func name  : getPressTemp,getHeight
 * processing : MS5611の生データから温度TEMP、気圧P、高度Heightを計算
 * param      : ADC1,ADC2    / getD1,getD2で得られたD1,D2
 *              hPa          / getPressTempで得られたPをhPaにしたもの
 * summary    : MS5611のデータシートによる計算,と高度気圧式から線形化
 * return     : 
 */
void getPressTemp(uint32_t ADC1, uint32_t ADC2){
  dT   = (int32_t)(ADC2 - ((int32_t)C_[4] << 8));
  TEMP = 2000 + ((dT * (int64_t)C_[5]) >> 23);
  OFF  = (((int64_t)C_[1]) << 17) + (((int64_t)C_[3] * dT) >> 6);
  SENS = (((int64_t)C_[0]) << 16) + (((int64_t)C_[2] * dT) >> 7);
  P    = ((ADC1 * SENS) >> 21) - OFF >> 15;

  if (TEMP < 2000) {
    T2    = (dT * dT) >> 31;
    OFF2  = 61 * (TEMP - 2000) * (TEMP - 2000) >> 4;
    SENS2 = 2 * (TEMP - 2000) * (TEMP - 2000);
    if (TEMP < -1500) {
      OFF2  = OFF2 + 15 * (TEMP + 1500) * (TEMP + 1500);
      SENS2 = SENS2 + 8 * (TEMP + 1500) * (TEMP + 1500);
    }
    TEMP = TEMP - T2;
    OFF  = OFF - OFF2;
    SENS = SENS - SENS2;
  } 
}

void getHeight(int32_t hPa){
  //Height = -938.502 * hPa/100.0 + 948697; //t0=30[deg]で1000mまでの線形近似
  float t0 =30.0;float P0 = 1013.25;
  Height = 153.8*(t0+273.2)*(1-pow((hPa/100.0/P0),0.1902));
}

/*
 * func name  : startConv,readADC
 * processing : MS5611の内部ADCの変換開始関数
 *              変換後OSRの時間はアクセス不可
 *              ADCの結果を読み込む関数。returnでuint32_tが出てくる
 * param      : command    / D1,D2,OSRによってアドレスが異なる
 *                         / OSR(Over Sampling Ratio)             
 * summary    : startConv(command) -> delay(10) -> readADC()
 *              変換コマンドを送信
 *              値呼び出しコマンドを送信、受信
 * return     : 
 *              conversion    / ADCした値.D1,D2のこと
 */

void startConv(uint8_t command){
  Wire.beginTransmission(MS5607_ADDR);
  Wire.write(command);
  Wire.endTransmission();
}

uint32_t readADC(){
  uint32_t conversion = 0;
  // start read sequence
  Wire.beginTransmission(MS5607_ADDR);
  Wire.write(0x00);
  Wire.endTransmission();

  Wire.beginTransmission(MS5607_ADDR);
  Wire.requestFrom(MS5607_ADDR, 3); //3byteリクエスト
  if(Wire.available()){
    conversion = Wire.read() * 65536 + Wire.read() * 256 + Wire.read();
   }
  return conversion;
}

/*
 * func name  : readPROM
 * processing : MS5611の工場校正値を読み込み
 * param      : 
 * summary    : C_[i]に0~5読み込む(C1~C6とは一つズレている)
 * return     : 
 */
void readPROM(){ 
  for(int i=0; i<6; i++){
    Wire.beginTransmission(MS5607_ADDR);
    Wire.write(0xA2 + i*2); //PROMの最初 データシートでは0xA0に見えるが0xA2から   
    Wire.endTransmission();

    Wire.beginTransmission(MS5607_ADDR);
    Wire.requestFrom(MS5607_ADDR,2);
    if(Wire.available() >= 2){
      C_[i] = Wire.read() * 256 + Wire.read();
    }
  }
}
