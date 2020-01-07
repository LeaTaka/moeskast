#include <Wire.h>
#include <SeeedOLED.h>

void F_printOledSetup(){
  Wire.begin();
  SeeedOled.init();  //initialze SEEED OLED display
  
  SeeedOled.clearDisplay();          //clear the screen and set start position to top left corner
  SeeedOled.setNormalDisplay();      //Set display to normal mode (i.e non-inverse mode)
  SeeedOled.setHorizontalMode();     //Set addressing mode to Page Mode
  SeeedOled.setTextXY(0, 0);         //Set the cursor to Xth Page, Yth Column
  SeeedOled.putString("WaterTemp |");        //Print the String
  SeeedOled.setTextXY(1, 0);         //Set the cursor to Xth Page, Yth Column
  SeeedOled.putString("WaterEc   |");       //Print the String
  SeeedOled.setTextXY(2, 0);         //Set the cursor to Xth Page, Yth Column
  SeeedOled.putString("Waterpeil |");       //Print the String
  SeeedOled.setTextXY(3, 0);         //Set the cursor to Xth Page, Yth Column
  SeeedOled.putString("KasTemp   |");       //Print the String
  SeeedOled.setTextXY(4, 0);         //Set the cursor to Xth Page, Yth Column
  SeeedOled.putString("KasVocht  |");       //Print the String
  SeeedOled.setTextXY(5, 0);         //Set the cursor to Xth Page, Yth Column
  SeeedOled.putString("KasDruk   |");       //Print the String
  SeeedOled.setTextXY(6, 0);         //Set the cursor to Xth Page, Yth Column
  SeeedOled.putString("KamerTemp |");       //Print the String
  SeeedOled.setTextXY(7, 0);         //Set the cursor to Xth Page, Yth Column
  SeeedOled.putString("KamerVocht|");       //Print the String
}

void F_printOled(){
  SeeedOled.setTextXY(0, 11);       //Set the cursor to Xth Page, Yth Column
  SeeedOled.putFloat(Temperature); //Print the String
  SeeedOled.setTextXY(1, 11);       //Set the cursor to Xth Page, Yth Column
  SeeedOled.putFloat(ecStable);    //Print the String
  SeeedOled.setTextXY(2, 11);       //Set the cursor to Xth Page, Yth Column
  SeeedOled.putFloat(cmDist);      //Print the String
  SeeedOled.setTextXY(3, 11);                                       //Set the cursor to Xth Page, Yth Column
  SeeedOled.putFloat(bme680.sensor_result_value.temperature);      //Print the String
  SeeedOled.setTextXY(4, 11);                                       //Set the cursor to Xth Page, Yth Column
  SeeedOled.putFloat(bme680.sensor_result_value.humidity);         //Print the String
  SeeedOled.setTextXY(5, 11);                                       //Set the cursor to Xth Page, Yth Column
  SeeedOled.putFloat(bme680.sensor_result_value.pressure/ 100.0, 0); //Print the String
  SeeedOled.setTextXY(6, 11);       //Set the cursor to Xth Page, Yth Column
  SeeedOled.putFloat(T_mth02);     //Print the String
  SeeedOled.setTextXY(7, 11);       //Set the cursor to Xth Page, Yth Column
  SeeedOled.putFloat(H_mth02);     //Print the String
}
