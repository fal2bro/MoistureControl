#include "DHT.h"

// ピン定義
const int In1 = 5;
const int In2 = 6;
const int pressureSensorPin = A0;
#define DHTPIN 2     // what pin we're connected to


#define DHTTYPE DHT22   // DHT 22  (AM2302)
// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(DHTPIN, DHTTYPE);

// 圧力センサの仕様
const float sensorMinV = 1.0;  // センサの最小電圧（V）
const float sensorMaxV = 5.0;  // センサの最大電圧（V）
const float pressureMin = 0.0; // kPa
const float pressureMax = 1000.0; // kPa

// P制御
float targetHumid = 55.0 ;  // 目標圧力（kPa）
float Kp = 2.55;                // 比例ゲイン 湿度範囲が0~100なので、これを0~255にスケーリング
const float pressureLimit = 3.0;

void setup() {
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  
  dht.begin();
  Serial.begin(9600);
  digitalWrite(In2,LOW);
}

void loop() {

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // アナログ読み取り（0〜1023）
  int sensorValue = analogRead(pressureSensorPin);

  // 電圧に変換
  float voltage = sensorValue * (5.0 / 1023.0);

  // 圧力に変換
  float pressure = mapVoltageToPressure(voltage);

  // 誤差計算
  float error = targetHumid - h; //0~100

  // PWM出力の計算（P制御）
  int pwmValue = constrain(Kp * error, 0, 255);
  if (pressure >= pressureLimit){
    pwmValue = 0;
  }
  analogWrite(In1, pwmValue);

  // デバッグ出力
  Serial.print("Pressure (kPa): ");
  Serial.print(pressure);
  Serial.print(" | PWM: ");
  Serial.println(pwmValue);

  delay(100); // 更新周期
}

// 電圧→圧力（kPa）変換関数
float mapVoltageToPressure(float voltage) {
  if (voltage < sensorMinV) voltage = sensorMinV;
  if (voltage > sensorMaxV) voltage = sensorMaxV;
  return (voltage - sensorMinV) * (pressureMax - pressureMin) / (sensorMaxV - sensorMinV);
}
