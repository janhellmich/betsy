#include <Servo.h>

//Initialize servos
Servo servoRedGreen;
Servo servoYellowBlue;
Servo servoStart;

// Analog pin assignments
#define sensorRed 4    
#define sensorGreen 1   
#define sensorYellow 2 
#define sensorBlue 3

// define color constants
#define RED 1
#define GREEN 2
#define YELLOW 3
#define BLUE 4

#define PIN_START_SERVO 9
#define PIN_YELLOW_BLUE 11
#define PIN_RED_GREEN 10

#define SERVO_START_YELLOW_BLUE 52
#define SERVO_START_RED_GREEN 52
#define SERVO_START_MIDDLE 30
#define TURN_ANGLE 50

//Initialize variables

int pressedCount = 0;
int pressedArray [4] = {0};


int redSensor = 0;
int blueSensor = 0;
int greenSensor = 0;
int yellowSensor = 0;

// measured brighness values
int brightnessRed = 0;        
int brightnessGreen = 0;        
int brightnessYellow = 0;        
int brightnessBlue = 0;


// setpoints determined in calibration
int redSetpoint = 0;
int greenSetpoint = 0;
int yellowSetpoint = 0;
int blueSetpoint = 0;




/********************  SETUP  ***************************************************************************************************************/
void setup()
{
  Serial.begin(9600);

  //Set pins as inputs
  pinMode(sensorRed, INPUT);
  pinMode(sensorGreen, INPUT);
  pinMode(sensorYellow, INPUT);
  pinMode(sensorBlue, INPUT);
  
  //PWM pin assignments 
  servoRedGreen.attach(PIN_RED_GREEN);            
  servoYellowBlue.attach(PIN_YELLOW_BLUE);
  servoStart.attach(PIN_START_SERVO);

  //Servo Starting Angles  
  servoRedGreen.write(SERVO_START_RED_GREEN);              
  servoYellowBlue.write(SERVO_START_YELLOW_BLUE);
  servoStart.write(SERVO_START_MIDDLE);
}

/******************   MAIN LOOP   ***************************************************************************************************************/
void loop()
{

  //Determine the setpoint values
  Calibrate_Setpoints();
  
  delay(200);
  
  start_game();

  compare_brightness();   //Go to Function compare_brightness_1 to determine which color is lit and which button to press
  pressedCount++;
  
  for (int i = 0; i < pressedCount; i++)
  {    
    press_button(pressedArray[i]);
  }
  
  
  for (int i = 0; i < 50; i++)  
  {
    read_sensors();
    compare_brightness();
    delay(10);
  }
  
  pressedCount++;
 
  for (int i = 0; i < pressedCount; i++)
  {    
    press_button(pressedArray[i]);
  }
  
  for (int i = 0; i < 50; i++)  
  {
    read_sensors();
    compare_brightness();
    delay(10);
  }     
  
  pressedCount++;  
  
  for (int i = 0; i < pressedCount; i++)
  {    
    press_button(pressedArray[i]);
  }
  
  while(1) {}                                                                              

}


/******************   FUNCTIONS   ***************************************************************************************************************/
void compare_brightness()                                                    //Determine which color is the 1st illuminated
{
  int lastPress = 0;
  
  if (brightnessRed > redSetpoint)                                                            //If Red sensor shows brightness > Red Setpoint
  {
    lastPress = RED;
  }
  
  if (brightnessGreen > greenSetpoint)                                                        //If Green sensor shows brightness > Green Setpoint
  {
    lastPress = GREEN;                                                                              
  }
 
  if (brightnessYellow >yellowSetpoint)                                                       //If Yellow sensor shows brightness > Yellow Setpoint
  {
    lastPress = YELLOW;
  }

  if (brightnessBlue > blueSetpoint)                                                          //If Blue sensor shows brightness > Blue Setpoint
  {
    lastPress = BLUE;
  }
  if (lastPress != 0) 
  {
     pressedArray[pressedCount] = lastPress;
  }  
  
}

// This function reads the sensors 5 times, averages each one of them and returns their result with a buffer of 30 to account for error
int Calibrate_Setpoints()
{
  int i;  //Counter
  
  for (i=0; i < 5; i++)
  {
    redSensor = redSensor + analogRead(sensorRed);
    blueSensor = blueSensor + analogRead(sensorBlue);
    greenSensor = greenSensor + analogRead(sensorGreen);
    yellowSensor = yellowSensor + analogRead(sensorYellow);
    delay(200);                                                                                 //Increased delay to account for flash photography
  }

  //Average Readings and add a buffer of 30 to account for error
  redSensor = redSensor/5 + 30;
  blueSensor = blueSensor/5 + 30;
  greenSensor = greenSensor/5 + 30;
  yellowSensor = yellowSensor/5 + 30;  

  //Assign Setpoint Values from averages
  redSetpoint = redSensor;                        
  blueSetpoint = blueSensor;
  greenSetpoint = greenSensor;              
  yellowSetpoint = yellowSensor;
  
  Serial.println("Red Sensor Cal: " + String(redSetpoint)); 
  Serial.println("Green Sensor Cal: " + String(greenSetpoint));   
  Serial.println("Yellow Sensor Cal: " + String(yellowSetpoint)); 
  Serial.println("Blue Sensor Cal: " + String(blueSetpoint)); 
  
  
}


//Take all sensor readings
int read_sensors()
{
  brightnessRed = analogRead(sensorRed);                                                  
  brightnessGreen = analogRead(sensorGreen);
  brightnessYellow = analogRead(sensorYellow);
  brightnessBlue = analogRead(sensorBlue);
  
  Serial.println("Red Sensor: " + String(brightnessRed)); 
  Serial.println("Green Sensor: " + String(brightnessGreen));   
  Serial.println("Yellow Sensor: " + String(brightnessYellow)); 
  Serial.println("Blue Sensor: " + String(brightnessBlue)); 
  
}


//Press the appropriate button
void press_button(int button)
{
  int pos;
  //Press Red
  if (button == RED)                                                                              
  {
    for(pos = SERVO_START_RED_GREEN; pos >= SERVO_START_RED_GREEN - TURN_ANGLE ; pos -= 1)                                                    
    {
      servoRedGreen.write(pos);
      delay(15);
    }
    for(pos = SERVO_START_RED_GREEN - TURN_ANGLE; pos <= SERVO_START_RED_GREEN; pos += 1)
    {
      servoRedGreen.write(pos);
      delay(15);
    }
  }

  //Press Green
  if (button == GREEN)                                                                               
  {
    for(pos = SERVO_START_RED_GREEN; pos <= SERVO_START_RED_GREEN + TURN_ANGLE; pos += 1)                                                    
    {
      servoRedGreen.write(pos);
      delay(15);
    }
    for(pos = SERVO_START_RED_GREEN + TURN_ANGLE; pos >= SERVO_START_RED_GREEN; pos -= 1)
    {
      servoRedGreen.write(pos);
      delay(15);
    }
  }

  //Press Yellow
  if (button == YELLOW)                                                                              
  {
    for(pos = SERVO_START_YELLOW_BLUE; pos >= SERVO_START_YELLOW_BLUE - TURN_ANGLE; pos -= 1)                                                     
    {
      servoYellowBlue.write(pos);
      delay(15);
    }
    for(pos = SERVO_START_YELLOW_BLUE - TURN_ANGLE; pos <= SERVO_START_YELLOW_BLUE; pos += 1)
    {
      servoYellowBlue.write(pos);
      delay(15);
    }
  }

  //Press Blue
  if (button == BLUE)                                                                               
  {
    for(pos = SERVO_START_YELLOW_BLUE; pos <= SERVO_START_YELLOW_BLUE + TURN_ANGLE; pos += 1)                                                    
    {
      servoYellowBlue.write(pos);
      delay(15);
    }
    for(pos = SERVO_START_YELLOW_BLUE + TURN_ANGLE; pos >= SERVO_START_YELLOW_BLUE; pos -= 1)
    {
      servoYellowBlue.write(pos);
      delay(15);
    } 
  }
}



void start_game() 
{
    int pos;
    for(pos = 30; pos >= 1; pos -= 1)                                          //press Center Button to start game
    {
      servoStart.write(pos);
      delay(15);
    }
    
    read_sensors();
      
    for(pos = 1; pos <= 30; pos += 1)                                         //Release Start Button
    {
      servoStart.write(pos);
      delay(15);
    }
}


