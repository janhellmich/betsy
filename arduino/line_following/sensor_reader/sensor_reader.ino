#include <QTRSensors.h>
#include <LiquidCrystal.h>
// This example is designed for use with eight QTR-1RC sensors or the eight sensors of a
// QTR-8RC module.  These reflectance sensors should be connected to digital inputs 3 to 10.
// The QTR-8RC's emitter control pin (LEDON) can optionally be connected to digital pin 2, 
// or you can leave it disconnected and change the EMITTER_PIN #define below from 2 to 
// QTR_NO_EMITTER_PIN.

// The main loop of the example reads the raw sensor values (uncalibrated).
// You can test this by taping a piece of 3/4" black electrical tape to a piece of white 
// paper and sliding the sensor across it.  It prints the sensor values to the serial 
// monitor as numbers from 0 (maximum reflectance) to 2500 (minimum reflectance).


#define NUM_SENSORS   8     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   1     // emitter is controlled by digital pin 2
#define RESISTOR_PIN 0


// sensors 0 through 7 are connected to digital pins 3 through 10, respectively
QTRSensorsRC qtrrc((unsigned char[]) {31,32,33,34,35,36,39,40},
  NUM_SENSORS, TIMEOUT ); 
unsigned int sensorValues[NUM_SENSORS];

LiquidCrystal lcd(43, 45, 47, 49, 51, 53);
  
void setup()
{
  delay(500);
  lcd.begin(16, 2);
  Serial.begin(9600); // set the data rate in bits per second for serial data transmission
  delay(1000);
  
}


void loop()
{
  Serial.println(analogRead(0));
  // read raw sensor values
  qtrrc.read(sensorValues, QTR_EMITTERS_OFF);
  lcd.setCursor(0,1);
  lcd.clear();
  lcd.print(sensorValues[7]);
  // print the sensor values as numbers from 0 to 2500, where 0 means maximum reflectance and
  // 1023 means minimum reflectance
//  for (unsigned char i = 0; i < NUM_SENSORS; i++)
//  {
//    Serial.print(sensorValues[i]);
//    Serial.print('\t'); // tab to format the raw data into columns in the Serial monitor
//  }
//  Serial.println();
  
  delay(500);
}