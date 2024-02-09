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

BluetoothSerial SerialBT;
int PIN_DATA = 18;  ///< SPI Data pin number
int PIN_CLK = 17;   ///< SPI Clock pin number
int PIN_FSYNC = 5;  ///< SPI Load pin number
String S = "";
int16_t adc0, adc1;
MD_AD9833 AD(PIN_DATA, PIN_CLK, PIN_FSYNC); // Arbitrary SPI pins

Adafruit_ADS1115 ads;

const float VREF = 4.096;  // Voltage reference
const float GAIN = 1.0;    // ADC gain

void setup(void)
{
  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.begin(115200);  // Increase baud rate for faster communication
  SerialBT.begin("EDDY_C");
  ads.begin();
  ads.setGain(GAIN_ONE);  // Set the gain (1x) for the ADS1115
  AD.begin();
  AD.setMode(MD_AD9833::MODE_OFF);
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

  adc0 = ads.readADC_SingleEnded(0);
  adc1 = ads.readADC_SingleEnded(1);

  float millivolts_adc0 = adc0 * (VREF / 32767) * GAIN * 1000;
  float millivolts_adc1 = adc1 * (VREF / 32767) * GAIN *1000;

  Serial.print("ADC 0 : ");
  Serial.println(millivolts_adc0);
  Serial.print("ADC 1 : ");
  Serial.println(millivolts_adc1);

  delay(10);  // Adjust delay for the desired sample rate
}
