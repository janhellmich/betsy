// This program will read in the photoresistor to start the game

#define RESISTOR_PIN 0

void setup() 
{
  delay(500);
  Serial.begin(9600);
  delay(1000);
}

void loop() 
{
  Serial.println(analogRead(RESISTOR_PIN)); 
  delay(250);
}
