#include "BMP280.h"

long signed int BMP280::bmp280_compensate_T_int32(long signed int adc_T)
{
  long signed int var1, var2, T;
  var1 = ((((adc_T >> 3) - ((long signed int)DIG_T1 << 1))) * ((long signed int)DIG_T2)) >> 11;
  var2 = (((((adc_T >> 4) - ((long signed int)DIG_T1)) * ((adc_T >> 4) - ((long signed int)DIG_T1))) >> 12) *
          ((long signed int)DIG_T3)) >>
         14;
  t_fine = var1 + var2;
  T = (t_fine * 5 + 128) >> 8;
  return T;
}

// Set the global DIG_T1, DIG_T2, and DIG_T3.
void BMP280::getTrimmingPoints()
{
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_DIG_T1_LSB_REGISTER_ADDRESS);
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS, 6); // 6 bytes, 2 for each trimming point
  if (Wire.available() != 6)
  {
    Serial.print("Error: Expected 6 bytes but got ");
    Serial.println(Wire.available());
  }
  unsigned char readings[6];
  for (int i = 0; i < 6; ++i)
  {
    readings[i] = Wire.read();
  }
  DIG_T1 = (readings[1] << 8) | readings[0];
  DIG_T2 = (readings[3] << 8) | readings[2];
  DIG_T3 = (readings[5] << 8) | readings[4];
}

// Returns data stored in the ID register (address 0xD0) if available, else NULL
unsigned char BMP280::getBMP280ID()
{
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_ID_REGISTER_ADDRESS);
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS, 1);
  if (Wire.available() != 1)
  {
    Serial.print("Error: Expected 1 byte but got ");
    Serial.println(Wire.available());
    return NULL;
  }
  if (Wire.available())
  {
    return Wire.read();
  }
  return NULL;
}

// Returns the temperature in DegC
long signed int BMP280::getTempReading()
{
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_TEMP_MSB_REGISTER_ADDRESS); // Start at the most significant bit, and then read in 3 bytes for the least significant bit and xtended least significant bit
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS, 3);
  if (Wire.available() != 3)
  {
    Serial.print("Error: Expected 3 bytes but got ");
    Serial.println(Wire.available());
    return NULL;
  }
  unsigned char readings[3];
  for (int i = 0; i < 3; ++i)
  {
    readings[i] = Wire.read();
  }
  unsigned long ut = ((unsigned long)readings[0] << 12) | ((unsigned long)readings[1] << 4) | (readings[2] >> 4);
  long signed int tempDegC = bmp280_compensate_T_int32(ut);
  return tempDegC / 100.0;
}

void BMP280::setBMP280Configuration()
{
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_CTRL_MEAS_REGISTER_ADDRESS);
  Wire.write(BMP280_CONFIG);
  Wire.endTransmission();
  getTrimmingPoints();
}