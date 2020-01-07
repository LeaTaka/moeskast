#include "seeed_bme680.h"

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define IIC_ADDR  uint8_t(0x76)

/**  NOTE!!!!!!!!!!!!  Select the communication protocol correctly **/
Seeed_BME680 bme680(IIC_ADDR); /* IIC PROTOCOL */
//Seeed_BME680 bme680;             /* SPI PROTOCOL */
//Seeed_BME680 bme680(BME_CS, BME_MOSI, BME_MISO,  BME_SCK);/*SPI PROTOCOL*/

void F_bme680Setup(){
  while (!bme680.init()) 
  {
    Serial.println("bme680 init failed ! can't find device!");
    delay(10000);
  }
}

void F_bme680(){
  if (bme680.read_sensor_data()) 
  {
    Serial.println("Failed to perform reading :(");
    return;
  }
}
