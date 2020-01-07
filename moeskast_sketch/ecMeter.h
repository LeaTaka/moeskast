/*  ElCheapo Arduino EC-PPM measurments
  
    This scrip uses a common USA/EU two prong plug and a 47Kohm Resistor to measure the EC/PPM of a Aquaponics/Hydroponics Sytem.
    You could modify this code to Measure other liquids if you change the resitor and values at the top of the code.
    This Program will give you a temperature based feed controller.
    28/8/2015  Michael Ratcliffe  Mike@MichaelRatcliffe.com
    This program is free software: see <http://www.gnu.org/licenses/>.

    -1 kohm resistor
    -DS18B20 Waterproof Temperature Sensor */

//*** Libraries Needed To Compile The Script [See Read me In Download] ***//

// Both below Library are custom ones [ SEE READ ME In Downloaded Zip If You Dont Know how To install] Use them or add a pull up resistor to the temp probe

#include <OneWire.h>
#include <DallasTemperature.h>

/*             EC meter User Defined Variables              ↑↑
==============================================================
↓↓  Do not Replace R1 with a resistor lower than 300 ohms   */

int R1=1000;
int Ra=25; //Resistance of powering Pins
int ECPin=A0;
//int ECGround=A2;
int ECPower=A1;

//*** Converting to ppm [Learn to use EC it is much better] ***//
// Hana      [USA]        PPMconverion:   0.5
// Eutech    [EU]         PPMconversion:  0.64
// Tranchen  [Australia]  PPMconversion:  0.7
// Why didnt anyone standardise this?

float PPMconversion=0.64;

//*** Compensating for temperature ***//
//The value below will change depending on what chemical solution we are 
//measuring. 0.019 is generaly considered the standard for plant nutrients 
//[google "Temperature compensation EC" for more info

float TemperatureCoef=0.019; //this changes depending on what chemical we are measuring

//*** Cell Constant For Ec Measurements ***//
//Mine was around 2.9 with plugs being a standard size they should all be 
//around the same. But If you get bad readings you can use the calibration 
//script and fluid to get a better estimate for K.
//I Recommend Calibrating your probe but if that is not an option at this 
//time the following cell constants [K] will give a good estimated readout:
//EU plug: K=1.76
//US Plug K=2.88
float K=1.82;

//*** Temp Probe Related ***//

#define ONE_WIRE_BUS 4          //Data wire For Temp Probe is plugged into pin 10 on the Arduino
const int TempProbePossitive=8;  //Temp Probe power connected to pin 9
const int TempProbeNegative=9;   //Temp Probe Negative connected to pin 8

//*** END Of Recomended User Inputs ***//

OneWire oneWire(ONE_WIRE_BUS);// Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);// Pass our oneWire reference to Dallas Temperature.

float Temperaturecorrection=1.5;
float Temperature=10;
float EC=0;
float EC25=0;
float ECstable=0;
int ppm=0;
float raw=0;
float Vin=5;
float Vdrop=0;
float Rc=0;
float buffer=0;

// update cycle 9 = approx 55 seconds before probe value updates EC variable
int input;
int counter=500;
float ecStable=0;

/*              End of User Defined Variables               ↑↑
==============================================================
↓↓           Setup - runs Once and sets pins etc            */

void F_updateEcSetup(){
  pinMode(ECPin,INPUT);
  pinMode(ECPower,OUTPUT);//Setting pin for sourcing current
  
  delay(100);// gives sensor time to settle
  sensors.begin();
  //*** Add Digital Pin Resistance to [25 ohm] to the static Resistor ***
  R1=(R1+Ra);// Taking into acount Powering Pin Resitance

  //Serial.println("ElCheapo Arduino EC-PPM measurments. By Mike@MichaelRatcliffe.com");
  //Serial.println("Measurments at 5's Second intervals [Dont read Ec more than once every 5 seconds]:");
};

void F_getEC(){

  //*** Reading Temperature Of Solution ***//

  sensors.requestTemperatures(); //Send the command to get temperatures
  Temperature=(sensors.getTempCByIndex(0)+Temperaturecorrection); //Stores Value in Variable

  //*** Estimates Resistance of Liquid ***//

  digitalWrite(ECPower,HIGH);
  raw=analogRead(ECPin);
  digitalWrite(ECPower,LOW);

  //*** Converts to EC ***//

  Vdrop= (Vin*raw)/1023.0;
  Rc=(Vdrop*R1)/(Vin-Vdrop);
  Rc=Rc-Ra; //acounting for Digital Pin Resitance
  EC = 1000/(Rc*K);

  //*** Compensating For Temperaure ***//

  EC25=EC/ (1+ TemperatureCoef*(Temperature-25.0));
  ppm=(EC25)*(PPMconversion*1000);
};

void F_updateEc(){

  ++counter;
  if (counter >= 500) { //500 loops = approx 1 hour -sort of :)
    F_getEC(); // Calls GetEC loop. Dont call this more that 1/5 hhz [once every five seconds] or you will polarise the water
    delay(3300);
    if (counter >= 509) { //509 loops = enough EC readings to update current EC value with a stable EC reading
      ecStable = EC25;
      //reset counter
      counter=0;
    }
  }
};
