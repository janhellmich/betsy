
// Analog pin assignments
#define sensorRed 4    
#define sensorGreen 1   
#define sensorYellow 2 
#define sensorBlue 3


void setup()
{
  Serial.begin(9600);

  //Set pins as inputs
  pinMode(sensorRed, INPUT);
  pinMode(sensorGreen, INPUT);
  pinMode(sensorYellow, INPUT);
  pinMode(sensorBlue, INPUT);
  
}

void loop()
{
  
  Serial.println("Red Sensor: " + String(analogRead(sensorRed))); 
  Serial.println("Green Sensor: " + String(analogRead(sensorGreen)));   
  Serial.println("Yellow Sensor: " + String(analogRead(sensorYellow))); 
  Serial.println("Blue Sensor: " + String(analogRead(sensorBlue))); 
  
  delay(1000);
  
}

