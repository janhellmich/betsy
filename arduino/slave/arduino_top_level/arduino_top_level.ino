#include <Servo.h>

// This arduino is responsible to play the Rubik's Cube and the Simon Says game


// RUBIK'S CUBE CONSTANTS

// pin assignment 
#define PIN_RK_ARM 11
#define PIN_RK_TURN 10

#define RK_ARM_START 150
#define RK_ARM_FINISH 50

#define RK_TURN_START 3 
#define RK_TURN_FINISH 113

// SIMON SAYS CONSTANTS

// Analog pin assignments
#define sensorRed 2    
#define sensorGreen 4   
#define sensorYellow 3 
#define sensorBlue 1

// digital pin assignments
#define PIN_SS_ARM 3
#define PIN_START_SERVO 6
#define PIN_YELLOW_BLUE 9
#define PIN_RED_GREEN 5

// define SS plating servo angles
#define SERVO_START_YELLOW_BLUE 103
#define SERVO_START_RED_GREEN 88
#define SERVO_START_MIDDLE 60
#define TURN_ANGLE 45

// define SS arm angles
#define SS_ARM_START 165
#define SS_ARM_FINISH 60

// define photoresistor threshold values
#define THRESHOLD_RED 350
#define THRESHOLD_GREEN 290
#define THRESHOLD_YELLOW 290
#define THRESHOLD_BLUE 200

// define color constants
#define RED 2
#define GREEN 4
#define YELLOW 3
#define BLUE 1

// COMMUNICATION
#define SIGNAL_IN 12
#define SIGNAL_OUT 13

//Servo motor assignment
Servo rkArm;
Servo rkTurn;
Servo ssArm;
Servo ssStart;
Servo ssBlueYellow;
Servo ssGreenRed;

//Simon Says servos
Servo servoRedGreen;
Servo servoYellowBlue;
Servo servoStart;

Servo simonArm;

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

int lastPress = BLUE;
int maximum = 0;


void setup()
{
  Serial.begin(9600);
  
  //SIMON SAYS
  
  //Set pins as inputs
  pinMode(sensorRed, INPUT);
  pinMode(sensorGreen, INPUT);
  pinMode(sensorYellow, INPUT);
  pinMode(sensorBlue, INPUT);
  
  //PWM pin assignments 
  servoRedGreen.attach(PIN_RED_GREEN);            
  servoYellowBlue.attach(PIN_YELLOW_BLUE);
  servoStart.attach(PIN_START_SERVO);
  simonArm.attach(PIN_SS_ARM);


  //Servo Starting Angles  
  servoRedGreen.write(SERVO_START_RED_GREEN);              
  servoYellowBlue.write(SERVO_START_YELLOW_BLUE);
  servoStart.write(SERVO_START_MIDDLE);
  simonArm.write(SS_ARM_START);
  
  
  // define communication 
  pinMode(SIGNAL_IN, INPUT);
  pinMode(SIGNAL_OUT, OUTPUT);
  
  // initialize signal to low
  digitalWrite(SIGNAL_OUT, LOW);
  
  delay(1000);
  
  // attach RK servo
  rkArm.attach(PIN_RK_ARM);
  rkTurn.attach(PIN_RK_TURN);
  rkArm.write(RK_ARM_START);
  rkTurn.write(RK_TURN_START);
  
}

void loop()

{
//  // wait for signal to play Simon Says
  while (digitalRead(SIGNAL_IN) != HIGH)
  {}
  
  play_simon_says();
  
  // signal finished with game
  finished();
  
  // wait for signal to play Rubiks Cube
  Serial.print(digitalRead(SIGNAL_IN));
  while (digitalRead(SIGNAL_IN) != HIGH)
  {}
  
  play_rubiks_cube();
  // signal finished with game
  finished();
  
  while (true)
  {}
  
}


/********************* FUNCTIONS  ************************************************************************/



/********************* FINISHED  ************************************************************************/

void finished() 
{
  
  digitalWrite(SIGNAL_OUT, HIGH);
  delay(1000);
  digitalWrite(SIGNAL_OUT, LOW);
  
}

/********************* PLAY SIMON SAYS  ************************************************************************/

void play_simon_says()
{
  
  //lower arm
  for (int i = SS_ARM_START; i > SS_ARM_FINISH; i--)
  {
    simonArm.write(i);
    delay(15);
  }
  
  delay(200);

  //Determine the setpoint values
  Calibrate_Setpoints();
  
  start_game();
  
  delay(4500);
  
  press_button(lastPress); 
  
  delay(4500);
  
  press_button(lastPress);
  
  delay(4500);
  
  
  for (int i = SS_ARM_FINISH; i < SS_ARM_START; i++)
  {
    simonArm.write(i);
    delay(15);
  }
}

/******************  SIMON HELPER FUNCTIONS   ***************************************************************************************************************/
void compare_brightness()                                                    //Determine which color is the 1st illuminated
{
  
  if ((brightnessRed - redSetpoint) > maximum)                                                            //If Red sensor shows brightness > Red Setpoint
  {
    Serial.println("detected Red: " + String(brightnessRed - redSetpoint) + "   " + String(maximum));
    lastPress = RED;
    maximum = brightnessRed - redSetpoint;
  }
  
  else if ((brightnessGreen - greenSetpoint) > maximum)                                                        //If Green sensor shows brightness > Green Setpoint
  {
    Serial.println("detected Green: " + String(brightnessGreen - greenSetpoint) + "   " + String(maximum));
    lastPress = GREEN;     
    maximum = brightnessGreen - greenSetpoint;    
  }
 
  else if ((brightnessYellow - yellowSetpoint) > maximum)                                                       //If Yellow sensor shows brightness > Yellow Setpoint
  {
    Serial.println("detected Yellow: " + String(brightnessYellow - yellowSetpoint) + "   " + String(maximum));
    lastPress = YELLOW;
    maximum = brightnessYellow - yellowSetpoint;
  }

  else if ((brightnessBlue - blueSetpoint) > maximum)                                                          //If Blue sensor shows brightness > Blue Setpoint
  {
    Serial.println("detected Blue: " + String(brightnessBlue - blueSetpoint) + "   " + String(maximum));
    lastPress = BLUE;
    maximum = brightnessBlue - blueSetpoint;
  }
  
}

// This function reads the sensors 5 times, averages each one of them and returns their result with a buffer of 30 to account for error
int Calibrate_Setpoints()
{
  int i;  //Counter
  
  int iterations = 20;
  
  for (i = 0; i < iterations; i++)
  {
    redSensor = redSensor + analogRead(sensorRed);
    blueSensor = blueSensor + analogRead(sensorBlue);
    greenSensor = greenSensor + analogRead(sensorGreen);
    yellowSensor = yellowSensor + analogRead(sensorYellow);
    delay(100);                                                                                 //Increased delay to account for flash photography
  }

  // add threshold to account for errors
  redSensor = redSensor/iterations + THRESHOLD_RED;
  blueSensor = blueSensor/iterations + THRESHOLD_BLUE;
  greenSensor = greenSensor/iterations + THRESHOLD_GREEN;
  yellowSensor = yellowSensor/iterations + THRESHOLD_YELLOW;  

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
  
  //Serial.println("Red Sensor: " + String(brightnessRed)); 
  //Serial.println("Green Sensor: " + String(brightnessGreen));   
  //Serial.println("Yellow Sensor: " + String(brightnessYellow)); 
  //Serial.println("Blue Sensor: " + String(brightnessBlue)); 
  
}


//Press the appropriate button
void press_button(int button)
{
  
  Serial.println(button);
  
  Serial.println("Red Sensor: " + String(brightnessRed)); 
  Serial.println("Green Sensor: " + String(brightnessGreen));   
  Serial.println("Yellow Sensor: " + String(brightnessYellow)); 
  Serial.println("Blue Sensor: " + String(brightnessBlue)); 
 
  int pos;
  //Press Red
  if (button == RED)                                                                              
  {
    
    Serial.println("________PRESSING RED NOW_________");
    for(pos = SERVO_START_RED_GREEN; pos >= SERVO_START_RED_GREEN - TURN_ANGLE ; pos -= 1)                                                    
    {
      servoRedGreen.write(pos);
      delay(10);
    }
    for(pos = SERVO_START_RED_GREEN - TURN_ANGLE; pos <= SERVO_START_RED_GREEN; pos += 1)
    {
      servoRedGreen.write(pos);
      delay(5);      
    }
    
  }

  //Press Green
  if (button == GREEN)                                                                               
  {
    
    Serial.println("________PRESSING GREEN NOW_________");
    for(pos = SERVO_START_RED_GREEN; pos <= SERVO_START_RED_GREEN + TURN_ANGLE; pos += 1)                                                    
    {
      servoRedGreen.write(pos);
      delay(10);
    }
    for(pos = SERVO_START_RED_GREEN + TURN_ANGLE; pos >= SERVO_START_RED_GREEN; pos -= 1)
    {
      servoRedGreen.write(pos);
      delay(5);   
    }
    
  }

  //Press Yellow
  if (button == YELLOW)                                                                              
  {
    
    Serial.println("________PRESSING YELLOW NOW_________");
    for(pos = SERVO_START_YELLOW_BLUE; pos >= SERVO_START_YELLOW_BLUE - TURN_ANGLE; pos -= 1)                                                     
    {
      servoYellowBlue.write(pos);
      delay(10);
    }
    for(pos = SERVO_START_YELLOW_BLUE - TURN_ANGLE; pos <= SERVO_START_YELLOW_BLUE; pos += 1)
    {
      servoYellowBlue.write(pos);
      delay(5);
    }
    
  }

  //Press Blue
  if (button == BLUE)                                                                               
  {
    Serial.println("________PRESSING BLUE NOW_________");
    for(pos = SERVO_START_YELLOW_BLUE; pos <= SERVO_START_YELLOW_BLUE + TURN_ANGLE; pos += 1)                                                    
    {
      servoYellowBlue.write(pos);
      delay(10);
    }
    for(pos = SERVO_START_YELLOW_BLUE + TURN_ANGLE; pos >= SERVO_START_YELLOW_BLUE; pos -= 1)
    {
      servoYellowBlue.write(pos);
      delay(5);
    } 
    
  }
  
}



void start_game() 
{
  
    int pos;
    
    for(pos = SERVO_START_MIDDLE; pos <= SERVO_START_MIDDLE + 30; pos += 1)                                          //press Center Button to start game
    {
      servoStart.write(pos);
     
      delay(15);
    }
      
    for(pos = SERVO_START_MIDDLE + 30; pos >= SERVO_START_MIDDLE; pos -= 1)                                         //Release Start Button
    {
      read_sensors();
      compare_brightness();
      servoStart.write(pos);
      delay(10);
    }
    
}


/********************* PLAY RUBIK'S CUBE  ************************************************************************/

void play_rubiks_cube()
{
  
  // attach servos 
  rkArm.attach(PIN_RK_ARM);
  rkTurn.attach(PIN_RK_TURN);
  
  
  //lower arm
  for (int i = RK_ARM_START; i > RK_ARM_FINISH; i--)
  {
    rkArm.write(i);
    delay(15);
  }
  
  // turn row
  for (int i = RK_TURN_START; i <= RK_TURN_FINISH; i++)
  {
    rkTurn.write(i);
    delay(15);
  }
  
  // raise arm
  for (int i = RK_ARM_FINISH; i < RK_ARM_FINISH + 20; i++)
  {
    rkArm.write(i);
    delay(15);
  }
  
  // turn back
  for (int i = RK_TURN_FINISH; i >= RK_TURN_START; i--)
  {
    rkTurn.write(i);
    delay(15);
  }
  
  // lower arm
  for (int i = RK_ARM_FINISH + 20; i > RK_ARM_FINISH; i--)
  {
    rkArm.write(i);
    delay(15);
  }
  
  // turn row
  for (int i = RK_TURN_START; i <= RK_TURN_FINISH; i++)
  {
    rkTurn.write(i);
    delay(15);
  }
  
  // raise arm
  for (int i = RK_ARM_FINISH; i < RK_ARM_FINISH + 20; i++)
  {
    rkArm.write(i);
    delay(15);
  }
  
  // turn back
  for (int i = RK_TURN_FINISH; i >= RK_TURN_START; i--)
  {
    rkTurn.write(i);
    delay(15);
  }
  
  rkTurn.detach();
  
  // raise arm
  for (int i = RK_ARM_FINISH + 20; i < RK_ARM_START; i++)
  {
    rkArm.write(i);
    delay(15);
  }
  
  rkArm.detach();
    
}
