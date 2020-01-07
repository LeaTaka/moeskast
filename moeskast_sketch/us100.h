#include <SoftwareSerial.h>;

const int US100_TX = 2;
const int US100_RX = 3;

//Serie instances nieuwe kanalen
SoftwareSerial portUS100(US100_RX, US100_TX);
 
unsigned int MSByteDist = 0;
unsigned int LSByteDist = 0;
unsigned int mmDist = 0;
unsigned int cmDist = 0;
int temp = 0;

void F_us100Setup(){
  portUS100.begin(9600);
}

void F_us100(){
 
    portUS100.flush(); // clean the serial port buffer
    portUS100.write(0x55); // order of distance measurement
 
    delay(500);
 
    if(portUS100.available() >= 2) // check the receipt of 2 bytes
    {
        MSByteDist = portUS100.read(); // reading of both bytes
        LSByteDist  = portUS100.read();
        mmDist  = MSByteDist * 256 + LSByteDist; // distance in mm
        cmDist  = 42 - (mmDist/10); // waterlevel in cm
        if((mmDist > 1) && (mmDist < 10000)) // Checking the distance within range
        {
            //Serial.print("Distance: ");
            //Serial.print(mmDist, DEC);
            //Serial.println(" mm");
        }
    }
 
    portUS100.flush(); // clean the serial port buffer
    portUS100.write(0x50); // order of distance measurement
 
    delay(500);
    if(portUS100.available() >= 1) // check the reception of 1 byte
    {
        temp = portUS100.read(); // read 1 byte
        if((temp > 1) && (temp < 130)) // valid range check
        {
            temp -= 45; // corrects a 45º offset
            //Serial.print("Temperature: ");
            //Serial.print(temp, DEC);
            //Serial.println(" ºC."); */
        }
    }
 
    delay(500);
};
