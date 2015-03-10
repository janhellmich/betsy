/*
  This program uses PD control to follow a white line on a black background.
  The SeeedStudio Motorshield V2.2 on an Arduino UNO was used.
*/  

#include <QTRSensors.h>
// define controller constants for error calculation
#define KP 0.05                     // Proportional Control Constant
#define KD 50                     // Derivative Control Constant. ( Note: KP < KD)

//define Max- and Basespeed
#define MAXSPEED 120          
#define BASESPEED 80         

//define line following set-up
#define NUM_SENSORS  4             // Number of sensors used to follow a straight line
#define NUM_POLLING_SENSORS 2      // Number of sensors used to poll for a 90 degree turn
#define TIMEOUT       2500         // Waits for 2500 us for sensor outputs to go low. If they are not yet low, value is set at 2500
#define EMITTER_PIN   1            // Emitter is controlled by digital pin 1

//define constants for motor shield pin assignments
#define RIGHTMOTORFORWARD 12        // Defined by the MotorShield. Do not use these pins for anything else.
#define RIGHTMOTORBACKWARD 13       // MotorShield
#define RIGHTMOTORPWM 10            // MotorShield
#define LEFTMOTORFORWARD 8          // MotorShield
#define LEFTMOTORBACKWARD 11        // MotorShield
#define LEFTMOTORPWM 9              // MotorShield

//define constants for motor control
#define RIGHTMOTOR 1
#define LEFTMOTOR 0
#define FWD 1
#define BWD 0
#define RIGHT 1
#define LEFT 0

// sensor set-up according to QTR library
QTRSensorsRC qtrrc((unsigned char[]) { 3, 4, 5, 6, } ,NUM_SENSORS, TIMEOUT, EMITTER_PIN);  // The 4 sensors used for following a straight line are digital pins 3, 4, 5, and 6
QTRSensorsRC poll((unsigned char[]) { 2, 7} ,NUM_POLLING_SENSORS, TIMEOUT, EMITTER_PIN);   // The 2 polling sensors for 90 degree turns are digital pins 2 and 7
unsigned int sensorValues[NUM_SENSORS];                                                    // An array containing the sensor values for the 4 line following sensors
unsigned int pollingValues[NUM_POLLING_SENSORS];                                           // An array containing the sensor values for the 2 polling sensors
boolean polling[2];

/********************  SETUP  ***************************************************************************************************************/

void setup()
{
  setupMotorshield();                                         // Jump to setupMotorshield to define pins as output
  auto_calibrate();                                           // function that calibrates the line following sensor
}

// Initialize error constant and motor speeds
int lastError = 0;      
int rightMotorSpeed = 0;
int leftMotorSpeed = 0;

/******************   MAIN LOOP   ***************************************************************************************************************/

void loop()
{
  // check for upcoming turns
  poll.read(pollingValues);
  for (unsigned char i = 0; i < (NUM_POLLING_SENSORS); i++)             
  {
    if(pollingValues[i] <= 500) polling[i] = 1;                         
    else polling[i] = 0;                                               
  }
  
  if ((polling[0] == 1))                            // Check to see if there is a 90 degree turn to the right
  {
    stop_motors();                                  // turn robot
    turn(LEFT);
    delay(3000);                                   
  }
  if ((polling[1] == 1))                            // Check to see if there is a 90 degree turn to the left
  {
    stop_motors();                                   // turn robot
    turn(RIGHT);
    delay(3000);                                        
  }
  // If there is no detected line on either polling sensor, continue with the PD Line Following
  {  
    int position = qtrrc.readLine(sensorValues,1,1);                       // Get calibrated readings along with the line position
    int error = position - 1500;                                           // Determine the error from the calculated position
    
    int motorSpeed = KP * error + KD * (error - lastError);                // Adjust motorspeed according to constants KP and KD
    lastError = error;                                                     // Update last error to compare to next error
  
    rightMotorSpeed = BASESPEED + motorSpeed;                              // Update right and left motor speeds by the calculated motor speed
    leftMotorSpeed = BASESPEED - motorSpeed;
    
    if (rightMotorSpeed > MAXSPEED ) rightMotorSpeed = MAXSPEED ;          // Prevent the motor from going beyond max speed
    if (leftMotorSpeed > MAXSPEED ) leftMotorSpeed = MAXSPEED;             // Prevent the motor from going beyond max speed
    if (rightMotorSpeed < 0) rightMotorSpeed = 0;                          // Keep the motor speed positive
    if (leftMotorSpeed < 0) leftMotorSpeed = 0;                            // Keep the motor speed positive                           
  }
  
  {
    // use drive function to with corrected motor speeds
    drive_motor(RIGHTMOTOR, FWD, rightMotorSpeed); 
    drive_motor(LEFTMOTOR, FWD, leftMotorSpeed);    
  }
}
/**********************************************************************************/
// this function sets up the motorshield
void setupMotorshield()
{
  pinMode(RIGHTMOTORFORWARD, OUTPUT);
  pinMode(RIGHTMOTORBACKWARD, OUTPUT);
  pinMode(RIGHTMOTORPWM, OUTPUT);
  pinMode(LEFTMOTORFORWARD, OUTPUT);
  pinMode(LEFTMOTORBACKWARD, OUTPUT);
  pinMode(LEFTMOTORPWM, OUTPUT);
  
  digitalWrite(RIGHTMOTORFORWARD, HIGH);
  digitalWrite(RIGHTMOTORBACKWARD, LOW);
  digitalWrite(LEFTMOTORFORWARD, HIGH);
  digitalWrite(LEFTMOTORBACKWARD, LOW);
}

// This function auto calibrated the sensor without having to move all sensors over white and black.
void auto_calibrate()
{
  // calibrate for 1 second. calibration values are passed to qtrrc.calibtratedMinimumOn/ qtrcc.calibratedMaximumOn
  for (int i = 0; i < 100; i++)
  {
    qtrrc.calibrate();
    delay(20);
  }
  // initialize min and max calibration values
  int calMin = 2500;
  int calMax = 0;
  // find the Min and Max values of the calibration
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    if (qtrrc.calibratedMinimumOn[i] < calMin) 
    {
      calMin = qtrrc.calibratedMinimumOn[i];
    }
    if (qtrrc.calibratedMaximumOn[i] > calMax) 
    {
      calMax = qtrrc.calibratedMaximumOn[i];
    }
  }
  // assign the min and max values to calibtration arrays
  for (unsigned char i = 0; i < NUM_SENSORS; i++)
  {
    qtrrc.calibratedMinimumOn[i] = calMin;
    qtrrc.calibratedMaximumOn[i] = calMax;
  }
}

// this function drives the motors: specify motor, direction, speed
void drive_motor(boolean motor, boolean dir, int spd)
{
  if (motor == RIGHTMOTOR)
  {
    if (dir == FWD) //Right motor forward
    {
      digitalWrite(RIGHTMOTORFORWARD, HIGH);
      digitalWrite(RIGHTMOTORBACKWARD, LOW);
      analogWrite(RIGHTMOTORPWM, spd);                            // Send the new calculated motor speeds from the motor controller to the motors
      
    }
    else if(dir == BWD)//Right motor backwards
    {
      digitalWrite(RIGHTMOTORFORWARD, LOW);
      digitalWrite(RIGHTMOTORBACKWARD, HIGH);
      analogWrite(RIGHTMOTORPWM, spd);                            // Send the new calculated motor speeds from the motor controller to the motors
    }
  }
  else if(motor == LEFTMOTOR)
  {
    if (dir == FWD)//Left motor forwards
    {
      digitalWrite(LEFTMOTORFORWARD, HIGH);
      digitalWrite(LEFTMOTORBACKWARD, LOW);
      analogWrite(LEFTMOTORPWM, spd);                            // Send the new calculated motor speeds from the motor controller to the motors
      
    }
    else if(dir == BWD)//Right motor backwards
    {
      digitalWrite(LEFTMOTORFORWARD, LOW);
      digitalWrite(LEFTMOTORBACKWARD, HIGH);
      analogWrite(LEFTMOTORPWM, spd);                            // Send the new calculated motor speeds from the motor controller to the motors
    }
  }  
}

// stop both motors
void stop_motors()
{
 drive_motor(RIGHTMOTOR, FWD, 0);
 drive_motor(LEFTMOTOR, FWD, 0);
}

// initiate a turn: specify direction
void turn(int dir)
{
  if (dir == RIGHT)
  {
    drive_motor(RIGHTMOTOR, FWD, 50);
    drive_motor(LEFTMOTOR, BWD, 50);
    delay(1000);
    while (true)
    {
      poll.read(pollingValues);
      qtrrc.read(sensorValues);
      if (pollingValues[0] < 500 )
      {
        stop_motors();
        break;
      }
    }
    
    delay(2000);
  }
  else if (dir == LEFT)
  {
    drive_motor(RIGHTMOTOR, BWD, 50);
    drive_motor(LEFTMOTOR, FWD, 50);
    poll.read(pollingValues);
    delay(500);
    while (pollingValues[0] < 500) 
    {
      poll.read(pollingValues);
      delay(100);
    }
    while (true)
    {
      poll.read(pollingValues);
      if (pollingValues[0] < 500 )
      {
        stop_motors();
        break;
      }
      delay(20);
    }
    drive_motor(RIGHT, FWD, 50);
    drive_motor(LEFT, FWD, 50);
    delay(2000);
  }
    
}

