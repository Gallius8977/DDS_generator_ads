#include <BluetoothSerial.h>
#include <Adafruit_ADS1X15.h>
#include <Wire.h>

#define SDA_PIN 23
#define SCL_PIN 19

#include <MD_AD9833.h>
#include <SPI.h>

#include "BluetoothSerial.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
int PIN_DATA = 17;  ///< SPI Data pin number
int PIN_CLK = 18;   ///< SPI Clock pin number
int PIN_FSYNC = 5;  ///< SPI Load pin number
String S = "";
int16_t adc0, adc1;
MD_AD9833 AD(PIN_DATA, PIN_CLK, PIN_FSYNC); // Arbitrary SPI pins

Adafruit_ADS1115 ads;

const float VREF = 4.096;  // Voltage reference
const float GAIN = 1.0;    // ADC gain

void setup(void)
{
  pinMode(22, OUTPUT);
  digitalWrite(22, LOW);
  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.begin(115200);
  SerialBT.begin("NoConn");
  ads.begin();
  ads.setGain(GAIN_ONE);
  AD.begin();
  AD.setMode(MD_AD9833::MODE_OFF);
  AD.setMode(MD_AD9833::MODE_SINE);
  AD.setFrequency(MD_AD9833::CHAN_0, 15000);
}

void loop(void)
{
  if (Serial.available() > 0) {
    S = Serial.readString();
    S.trim();
    Serial.println(S);
    int v = S.toInt();
    AD.setMode(MD_AD9833::MODE_SINE);
    AD.setFrequency(MD_AD9833::CHAN_0, v);
  }

  if (SerialBT.available() > 0) {
    S = SerialBT.readString();
    S.trim();
    SerialBT.println(S);
    Serial.println(S);
    int v = S.toInt();
    AD.setMode(MD_AD9833::MODE_SINE);
    AD.setFrequency(MD_AD9833::CHAN_0, v);
  }
  AD.setMode(MD_AD9833::MODE_SINE);
  AD.setFrequency(MD_AD9833::CHAN_0, 15000);
  float A=0;
  float B=0;
  for(int i=0;i<200;i++){
    adc0 = ads.readADC_SingleEnded(0);
    adc1 = ads.readADC_SingleEnded(1);
  
    float millivolts_adc0 = adc0 * (VREF / 32767) * GAIN * 1000;
    float millivolts_adc1 = adc1 * (VREF / 32767) * GAIN *1000;
  
  A=A+millivolts_adc0;
  B=B+millivolts_adc1;
  }
  A=A/200;
  B=B/200;
  Serial.println(A);
  Serial.println(B);
  float difference = B/A;
  int diff = difference*10000;
  SerialBT.println(diff);
  Serial.println(diff);
  delay(10);
}
