// SDカードモジュールのインクルード
#include <SD.h> //Load SD card library
#include<SPI.h> //Load SPI Library

// SDデータオブジェクトの定義, デジタルピン指定
int chipSelect = 4;
File mySensorData;

// IC2ワイヤーライブラリのインポート
#include "Wire.h"

// MPU-6050のI2Cアドレスの定数定義
const int MPU_addr=0x68;

// BMP180のライブラリのインポート と オブジェクト作成
#include "Adafruit_BMP085.h"
Adafruit_BMP085 mySensor;

// 加速度/ジャイロ/温度/気温/気圧の変数定義
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ; // 今回は加速度のみ
float tempC, tempF, pressure; // 今回は気圧のみ



void setup(){
  Serial.begin(9600); // シリアルモーターの電源起動
  
  mySensor.begin(); // BMP180の初期化

  Wire.begin(); 
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1の登録
  Wire.write(0);     // MPU-6050の初期化と起動
  Wire.endTransmission(true);

  pinMode(10, OUTPUT); // D10ピンに出力宣言
  SD.begin(4); // SDカードリーダーの初期化
}


void loop() {

  pressure=mySensor.readPressure(); // 気圧の読み込み

  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)

  mySensorData = SD.open("DataTest.txt", FILE_WRITE);
  if (mySensorData) {
    Serial.print(" | Pre = "); Serial.print(pressure);
    Serial.println(" Pa.");
    Serial.println("");
  
    Serial.print("AcX = "); Serial.print(AcX);
    Serial.print(" | AcY = "); Serial.print(AcY);
    Serial.print(" | AcZ = "); Serial.print(AcZ);
  
    delay(250); //Pause between readings.
  
    mySensorData.print(AcX);
    mySensorData.print(","); 
    mySensorData.print(AcY); 
    mySensorData.print(","); 
    mySensorData.print(AcZ); 
    mySensorData.print(","); 
    mySensorData.println(pressure);
    mySensorData.close();  
  }
}
