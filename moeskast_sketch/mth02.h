/*
Pins connection
Sensor(white box) upside, white box from you
First left pin - VDD(5+)
Second left pin - VSS(gnd)
Third left pin - empty
Fourth left pin - DATA
*/
#include <MTH02.h>
#define MTH_PIN 6
MTH mth(MTH_PIN);
float T_mth02 = 0;
float H_mth02 = 0;

void F_mth02Setup(){
  mth.begin();
  delay(100);
};

void F_mth02(){
  T_mth02 = mth.readTemperature();
  H_mth02 = mth.readHumidity();
};
