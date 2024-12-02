#include <Arduino.h>
#include <BMP280.h>

BMP280 bmp280;

void setup()
{
  // Start the serial communication (REQUIRED)
  Serial.begin(115200);
  // Start the I2C communication (REQUIRED) -- Without this, the master (Arduino) will not be able to communicate with the I2C device (BMP280)
  Wire.begin();
  // Get the current BMP280 address to verify if BMP280 is found (OPTIONAL, BUT RECOMMENDED)
  unsigned char bmp280ID = bmp280.getBMP280ID();
  while (bmp280ID == NULL)
  {
    Serial.println("Unable to locate BMP280 id. Please check your connections.");
  }
  // Set the BMP280 configuration (REQUIRED)
  // Current configuration: Temp Oversampling x2, Pressure Oversampling Skipped, Normal Mode
  bmp280.setBMP280Configuration();
}

void loop()
{
  // Get the temperature reading from BMP280
  long signed int tempReading = bmp280.getTempReading(); // Returns Degrees in Celcius
  Serial.print("Temperature: ");
  Serial.print(tempReading);
  Serial.println(" C");
  delay(500);
}