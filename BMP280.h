#pragma once
#include <Arduino.h>
#include <Wire.h>

#define BMP280_ADDRESS 0x76
#define BMP280_ID_REGISTER_ADDRESS 0xD0
#define BMP280_TEMP_XLSB_REGISTER_ADDRESS 0xFC
#define BMP280_TEMP_LSB_REGISTER_ADDRESS 0xFB
#define BMP280_TEMP_MSB_REGISTER_ADDRESS 0xFA
#define BMP280_CTRL_MEAS_REGISTER_ADDRESS 0xF4
#define BMP280_DIG_T1_LSB_REGISTER_ADDRESS 0x88
#define BMP280_CONFIG 0b01001111

class BMP280
{
private:
  unsigned short DIG_T1;
  short DIG_T2;
  short DIG_T3;
  long signed int t_fine; // Can be used for pressure and temperature conversion
  long signed int bmp280_compensate_T_int32(long signed int adc_T);
  void getTrimmingPoints();

public:
  unsigned char getBMP280ID(); // Returns data stored in the ID register (address 0xD0) if available, else NULL
  long signed int getTempReading();
  void setBMP280Configuration();
};