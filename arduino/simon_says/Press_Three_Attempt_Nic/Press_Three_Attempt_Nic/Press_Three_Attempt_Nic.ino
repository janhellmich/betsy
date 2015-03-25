#include <Servo.h>

//Initialize servos
Servo servo1;
Servo servo2;
Servo servoStart;

//Analog pin assignments
#define sensorRed = 0;    
#define sensorGreen = 1;   
#define sensorYellow = 2; 
#define sensorBlue = 3;

//Initialize variables
int pos1 = 0;
int pos2 = 0;
int posStart = 0;
int press = 0;

int redSensor = 0;
int blueSensor = 0;
int greenSensor = 0;
int yellowSensor = 0;

int brightnessRed = 0;        
int brightnessGreen = 0;        
int brightnessYellow = 0;        
int brightnessBlue = 0;

int redSetpoint = 0;
int greenSetpoint = 0;
int yellowSetpoint = 0;
int blueSetpoint = 0;

const int RED = 1;
const int GREEN = 2;
const int YELLOW = 3;
const int BLUE = 4;

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
  servo1.attach(10);            
  servo2.attach(11);
  servoStart.attach(9);

  //Servo Starting Angles  
  servo1.write(35);              
  servo2.write(35);
  servoStart.write(30);
}

/******************   MAIN LOOP   ***************************************************************************************************************/
void loop()
{

  //Determine the setpoint values
  Calibrate_Setpoints();
  
  delay(200);
  
   for(posStart = 30; posStart >= 1; posStart -= 1)                                          //press Center Button to start game
    {
      servoStart.write(posStart);
      delay(15);
    }
      
    for(posStart = 1; posStart <= 30; posStart += 1)                                         //Release Start Button
    {
      servoStart.write(posStart);
      delay(15);
    }

  Read_Sensors();
  Compare_Brightness(pos1, pos2);                                                            //Go to Function Compare_Brightness_1 to determine which color is lit and which button to press
  //Assign press value to Pressed[0]
  Pressed[0] = press;
  Press_Button(Pressed[0]);
  

  delay(725);                                                                                //Delay a certain amount of time to wait for 2nd color to illuminate
  Read_Sensors();
  Compare_Brightness(pos1, pos2);                                                            //Go to Function Compare_Brightness_2 to determine which color is lit and which button to press
  //Press 1st button again
  Press_Button();
  //Press 2nd button
  Press_Button();

  delay(1260);                                                                               //Delay a certain amount of time to wait for 3rd color to illuminate
  Read_Sensors();
  Compare_Brightness(pos1, pos2);                                                            //Go to Function Compare_Brightness_3 to determine which color is lit and which button to press
  Press_Button();
  
  while(1) {}                                                                                //Continuous loop (My way of ending the program)

}


/******************   FUNCTIONS   ***************************************************************************************************************/
int Compare_Brightness(int pos1, int pos2)                                                    //Determine which color is the 1st illuminated
{
 
  if (brightnessRed > redSetpoint)                                                            //If Red sensor shows brightness > Red Setpoint
  {
    press = RED;
  }
  
  if (brightnessGreen > greenSetpoint)                                                        //If Green sensor shows brightness > Green Setpoint
  {
    press = GREEN;                                                                              
  }
 
  if (brightnessYellow >yellowSetpoint)                                                       //If Yellow sensor shows brightness > Yellow Setpoint
  {
    press = YELLOW;
  }

  if (brightnessBlue > blueSetpoint)                                                          //If Blue sensor shows brightness > Blue Setpoint
  {
    press = BLUE;
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
    delay(500);                                                                                 //Increased delay to account for flash photography
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
}


//Take all sensor readings
int Read_Sensors()
{
  brightnessRed = analogRead(sensorRed);                                                  
  brightnessGreen = analogRead(sensorGreen);
  brightnessYellow = analogRead(sensorYellow);
  brightnessBlue = analogRead(sensorBlue);
}


//Press the appropriate button
void Press_Button(int press)
{
  //Press Red
  if (press == RED)                                                                              
  {
    for(pos1 = 35; pos1 >=1 ; pos1 -= 1)                                                    
    {
      servo1.write(pos1);
      delay(15);
    }
    for(pos1 = 1; pos1 <= 35; pos1 += 1)
    {
      servo1.write(pos1);
      delay(15);
    }
  }

  //Press Green
  if (press == GREEN)                                                                               
  {
    for(pos1 = 35; pos1 <= 70; pos1 += 1)                                                    
    {
      servo1.write(pos1);
      delay(15);
    }
    for(pos1 = 70; pos1 >= 35; pos1 -= 1)
    {
      servo1.write(pos1);
      delay(15);
    }
  }

  //Press Yellow
  if (press == YELLOW)                                                                              
  {
    for(pos2 = 35; pos2 >= 1; pos2 -= 1)                                                     
    {
      servo2.write(pos2);
      delay(15);
    }
    for(pos2 = 1; pos2 <= 35; pos2 += 1)
    {
      servo2.write(pos2);
      delay(15);
    }
  }

  //Press Blue
  if (press == BLUE)                                                                               
  {
    for(pos2 = 35; pos2 <= 70; pos2 += 1)                                                    
    {
      servo2.write(pos2);
      delay(15);
    }
    for(pos2 = 70; pos2 >= 35; pos2 -= 1)
    {
      servo2.write(pos2);
      delay(15);
    } 
  }
}

void Play_Game()
{
 if(turn == 0)
 {
  Press_Button(j[turn])
 }
}