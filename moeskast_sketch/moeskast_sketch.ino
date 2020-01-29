#include "mth02.h"
#include "ecMeter.h"
#include "bme680.h"
#include "us100.h"
#include "oled.h"

void setup(){
  
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Serial started.");
  delay(100);
  F_bme680Setup();
  F_updateEcSetup();
  F_us100Setup();
  F_mth02Setup();
  F_printOledSetup();
};

/*                        End of Setup                      ↑↑
==============================================================
                    Main Loop - Runs Forever                
     Moved Heavy Work To subroutines so you can call them
↓↓     from main loop without cluttering the main loop      */
   
void loop(){

  F_bme680();
  F_updateEc();
  F_us100();
  F_mth02();
  F_printOled();
  
  /*
  Serial.println("");
  Serial.print("|EC=");
  Serial.print(ecStable);
  Serial.print("|DS18B20=");
  Serial.print(Temperature);
  Serial.print("|D_US100=");
  Serial.print(cmDist, DEC);
  Serial.print("|T_US100=");
  Serial.print(temp, DEC);
  Serial.print("|T_BME680=");
  Serial.print(bme680.sensor_result_value.temperature);
  Serial.print("|P_BME680=");
  Serial.print(bme680.sensor_result_value.pressure/ 1000.0);
  Serial.print("|H_BME680=");
  Serial.print(bme680.sensor_result_value.humidity);
  Serial.print("|G_BME680=");
  Serial.print(bme680.sensor_result_value.gas/ 1000.0);
  Serial.print("|T_MTH02=");
  Serial.print(T_mth02,1);
  Serial.print("|H_MTH02=");
  Serial.print(H_mth02,1);
  Serial.println("|");
  */
  Serial.println("");
  Serial.print("{");
  Serial.print("\"EC\":");
  Serial.print(ecStable);
  Serial.print(",\"DS18B20\":");
  Serial.print(Temperature, 1);
  Serial.print(",\"D_US100\":");
  Serial.print(cmDist, DEC);
  Serial.print(",\"T_US100\":");
  Serial.print(temp, 1);
  Serial.print(",\"T_BME680\":");
  Serial.print(bme680.sensor_result_value.temperature, 1);
  Serial.print(",\"T_BME680DEC0\":");
  Serial.print(bme680.sensor_result_value.temperature* 100, 0);
  Serial.print(",\"P_BME680\":");
  Serial.print(bme680.sensor_result_value.pressure/ 100.0, 0);
  Serial.print(",\"H_BME680\":");
  Serial.print(bme680.sensor_result_value.humidity, 0);
  Serial.print(",\"G_BME680\":");
  Serial.print(bme680.sensor_result_value.gas/ 1000.0, 0);
  Serial.print(",\"T_MTH02\":");
  Serial.print(T_mth02, 1);
  Serial.print(",\"H_MTH02\":");
  Serial.print(H_mth02, 1);
  Serial.println("}");

  delay(2000);
};
