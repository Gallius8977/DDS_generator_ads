#include <Adafruit_ADS1X15.h>
#include <Wire.h>

#define SDA_PIN 25
#define SCL_PIN 26

#include <MD_AD9833.h>
#include <SPI.h>

#include "BluetoothSerial.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Pins for SPI comm with the AD9833 IC
BluetoothSerial SerialBT;
int PIN_DATA = 18;	///< SPI Data pin number
int PIN_CLK = 17;		///< SPI Clock pin number
int PIN_FSYNC = 5;	///< SPI Load pin number
String S = "";
MD_AD9833	AD(PIN_DATA, PIN_CLK, PIN_FSYNC); // Arbitrary SPI pins

Adafruit_ADS1115 ads;

void setup(void)
{
  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.begin(9600);
  SerialBT.begin("EDDY_C");
  ads.begin();
  ads.setGain(GAIN_TWO);
	AD.begin();
  AD.setMode(MD_AD9833::MODE_OFF);
}

void loop(void)
{
  if  (Serial.available() > 0) {
      S = Serial.readString() ;
      S.trim();
      Serial.println(S);
  	  int v =S.toInt();
      AD.setMode(MD_AD9833::MODE_SINE);
      AD.setFrequency(MD_AD9833::CHAN_0, v);
  }
  
  if  (SerialBT.available() > 0) {
      S = SerialBT.readString() ;
      S.trim();
      SerialBT.println(S);
      Serial.println(S);
  	  int v =S.toInt();
      AD.setMode(MD_AD9833::MODE_SINE);
      AD.setFrequency(MD_AD9833::CHAN_0, v);
  }
  int16_t adc0, adc1;
  adc0 = ads.readADC_SingleEnded(0);
  adc1 = ads.readADC_SingleEnded(1);
  Serial.print("ADC 0 : ");
  Serial.println(adc0);
  Serial.print("ADC 1 : ");
  Serial.println(adc1);
  delay(1000);
}
