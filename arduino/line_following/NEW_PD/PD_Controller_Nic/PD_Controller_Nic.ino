
/*
  This program uses PD control to follow a white line on a black background.
*/  

#include <QTRSensors.h>

// define controller constants for error calculation
#define KP .05                    // Proportional Control Constant
#define KD 50                       // Derivative Control Constant. ( Note: KP < KD)

//define Max- and Basespeed
#define MAXSPEED 75          
#define BASESPEED 70         

//define line following set-up
#define NUM_SENSORS 4             // Number of sensors used to follow a straight line
#define NUM_POLLING_SENSORS 2     // Number of sensors used to poll for a 90 degree turn
#define NUM_TURNING_SENSORS 2     // Number of sensors used to stop turns
#define TIMEOUT       2500        // Waits for 2500 us for sensor outputs to go low. If they are not yet low, value is set at 2500
#define EMITTER_PIN   30           // Emitter is controlled by digital pin 30

//define constants for motor shield pin assignments
#define STBY 24
#define RIGHTMOTORFORWARD 23        
#define RIGHTMOTORBACKWARD 22       
#define RIGHTMOTORPWM 2             
#define LEFTMOTORFORWARD 25         
#define LEFTMOTORBACKWARD 26        
#define LEFTMOTORPWM 3              

//define constants for motor control
#define RIGHTMOTOR 1
#define LEFTMOTOR 0
#define FWD 1
#define BWD 0
#define RIGHT 1
#define LEFT 0

// sensor set-up according to QTR library
QTRSensorsRC qtrrc((unsigned char[]) { 33, 34, 35, 36 } ,NUM_SENSORS, TIMEOUT, EMITTER_PIN);  // The 4 sensors used for following a straight line are digital pins 33, 34, 35, and 36
QTRSensorsRC poll((unsigned char[]) {38, 31} ,NUM_POLLING_SENSORS, TIMEOUT, EMITTER_PIN);    // The 2 polling sensors for 90 degree turns are digital pins 38 and 31
QTRSensorsRC turnIndicator((unsigned char[]) {39, 40} ,NUM_TURNING_SENSORS, TIMEOUT);        // The 2 polling sensors at the front are digital pins 39 and 40
unsigned int sensorValues[NUM_SENSORS];                                                      // An array containing the sensor values for the 4 line following sensors
unsigned int pollingValues[NUM_POLLING_SENSORS];                                             // An array containing the sensor values for the 2 polling sensors
unsigned int frontPollingValues[NUM_TURNING_SENSORS];                                        // An array containing the sensor values for the 2 front polling sensors


/********************  SETUP  ***************************************************************************************************************/

void setup()
{
  setupMotorshield();                                         // Jump to setupMotorshield to define pins as output
  auto_calibrate();                                           // function that calibrates the line following sensor
  Serial.begin(9600);
  Serial.println("Serial Activated");
}

// Initialize error constant and motor speeds
int lastError = 0;      
int rightMotorSpeed = 0;
int leftMotorSpeed = 0;

//Declare global last turn variable
boolean lastTurn;

/******************   MAIN LOOP   ***************************************************************************************************************/

void loop()
{
  
  //check for upcoming turns
  
  poll.read(pollingValues);									// Get polling sensor values
  if ((pollingValues[1] <= 500))                            // Check to see if there is a 90 degree turn to the right
  {   
    stop_motors();     
    delay(100);
    turnIndicator.read(frontPollingValues);                  // Read front sensors 
    if(frontPollingValues[0] <= 500 || frontPollingValues[1] <= 500) // Determine if the front sensors are seeing white
    {
      poll.read(pollingValues);
      if((pollingValues[0] <= 500)) //Check left sensor
      {
        Serial.println("Playing Game");    
        stop_motors();
        //Play Game
        delay(4000);	
        follow_bwd(lastTurn);
       }
       else
       {
         Serial.println("Game Turn");
         //Game Turn!
	 lastTurn = RIGHT;
         Serial.println(lastTurn);
	 turn(RIGHT);
	}
     }
     else //if front sensors did not see white
     {
       poll.read(pollingValues); //Read sensor values
       if((pollingValues[0] <= 500)) //Check left sensor
       {
	 Serial.println("T-Intersection");
         //T-Intersection
	 lastTurn = RIGHT;
         Serial.println(lastTurn);
      	 turn(RIGHT);
      	 //delay(2000);
        }
	else
	{
          Serial.println("Right Turn");
	  turn(RIGHT);
	}
      }
  }

  else if ((pollingValues[0] <= 500))                       // Check to see if there is a 90 degree turn to the left
  {   
    stop_motors();     
    delay(100);
    turnIndicator.read(frontPollingValues);                  // Read front sensors 
    if(frontPollingValues[0] <= 500 && frontPollingValues[1] <= 500) // Determine if the front sensors are seeing white
    {
      poll.read(pollingValues);
      if((pollingValues[1] <= 500)) //Check right sensor
      {
        Serial.println("Playing Game");
        stop_motors();
	//Play Game
	delay(4000);	
	follow_bwd(lastTurn);
      }
      else
      {
        Serial.println("Game Turn");
	lastTurn = LEFT;
        Serial.println(lastTurn);
	turn(LEFT); 
      }
    }
    else //if front sensors did not see white
    {
      poll.read(pollingValues);
      if((pollingValues[1] <= 500)) //Check right sensor
      {
        Serial.println("T-Intersection");
	//T-Intersection
	lastTurn = LEFT;
        Serial.println(lastTurn);
        turn(LEFT);        
      	//delay(2000);
      }
      else
      {
        Serial.println("Left Turn");
	turn(LEFT);
      }
     }
   }
  // If there is no detected line on either polling sensor, continue with the PD Line Following
  { 
     
    int positioning = qtrrc.readLine(sensorValues,1,1);                       // Get calibrated readings along with the line position
    int error = positioning - 1500;                                           // Determine the error from the calculated position
    
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
    // Use drive function with corrected motor speeds
    drive_motor(RIGHTMOTOR, FWD, rightMotorSpeed); 
    drive_motor(LEFTMOTOR, FWD, leftMotorSpeed);    
  }
}


/************************** FUNCTIONS ********************************************************/

/************************** SETUP MOTORSHIELD ************************************************/
// This function sets up the motorshield
void setupMotorshield()
{
  pinMode(STBY, OUTPUT);
  pinMode(RIGHTMOTORFORWARD, OUTPUT);
  pinMode(RIGHTMOTORBACKWARD, OUTPUT);
  pinMode(RIGHTMOTORPWM, OUTPUT);
  pinMode(LEFTMOTORFORWARD, OUTPUT);
  pinMode(LEFTMOTORBACKWARD, OUTPUT);
  pinMode(LEFTMOTORPWM, OUTPUT);
  
  digitalWrite(STBY, HIGH);
  digitalWrite(RIGHTMOTORFORWARD, HIGH);
  digitalWrite(RIGHTMOTORBACKWARD, LOW);
  digitalWrite(LEFTMOTORFORWARD, HIGH);
  digitalWrite(LEFTMOTORBACKWARD, LOW);
}

/*************************** AUTO-CALIBRATE *****************************************************************************/
// This function auto calibrates the sensor without having to move all sensors over white and black.
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

/************************** DRIVE MOTORS **********************************************************************/
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

/************************* STOP MOTORS ***************************************************************/
// stop both motors
void stop_motors()
{
  Serial.println("Stopping Motors");
  drive_motor(RIGHTMOTOR, FWD, 0);
  drive_motor(LEFTMOTOR, FWD, 0);
}

/************************** RESET MOTOR SPEEDS ********************************************************************/
// reset motor speeds
void reset_motor_speeds()
{
  rightMotorSpeed = BASESPEED;
  leftMotorSpeed = BASESPEED;
}

/************************** TURNS ********************************************************************/
// initiate a turn: specify direction
void turn(boolean dir)
{
  if (dir == RIGHT)
  {
    drive_motor(RIGHTMOTOR, BWD, 50);
    drive_motor(LEFTMOTOR, FWD, 50);
    turnIndicator.read(frontPollingValues);
    while (frontPollingValues[1] < 800 && frontPollingValues[0] < 800) 
    {
      turnIndicator.read(frontPollingValues);
      delay(100);
    }
    while (true)
    {
      turnIndicator.read(frontPollingValues);
      if (frontPollingValues[0] < 500 && frontPollingValues[1] < 500)
      {
        delay(20);            //This delay helps avoid false positives
        stop_motors();
        break;
      }
      //delay(20);
    }
    drive_motor(RIGHTMOTOR, FWD, 50);
    drive_motor(LEFTMOTOR, FWD, 50);
    delay(300);
    reset_motor_speeds();
  }
  
  else if (dir == LEFT)
  {
    drive_motor(RIGHTMOTOR, FWD, 50);
    drive_motor(LEFTMOTOR, BWD, 50);
    turnIndicator.read(frontPollingValues);
    while (frontPollingValues[0] < 800 && frontPollingValues[1] < 800) 
    {
      turnIndicator.read(frontPollingValues);
      delay(100);
    }
    while (true)
    {
      turnIndicator.read(frontPollingValues);
      if (frontPollingValues[0] < 500 && frontPollingValues[1] < 500)
      {
        delay(20);                      //This delay helps avoid false positives
        stop_motors();
        break;
      }
      //delay(20);
    }
    drive_motor(RIGHTMOTOR, FWD, 50);
    drive_motor(LEFTMOTOR, FWD, 50);
    delay(300);
    reset_motor_speeds();
  }
}
  
  /************************** FOLLOW LINE BWDS ********************************************************************/
//function to follow line bwds after playing a game
void follow_bwd(boolean dir)
{
  Serial.println("BWD Function");
  while (pollingValues[0] < 500 || pollingValues[1] < 500)
  {
    Serial.println("The var lastTurn is");
    Serial.print(lastTurn);
    poll.read(pollingValues);
    drive_motor(RIGHTMOTOR, BWD, 50);
    drive_motor(LEFTMOTOR, BWD, 50);
  }
  reset_motor_speeds();

 while (true)
 {
    poll.read(pollingValues);
    if ((pollingValues[0] < 500 || pollingValues[1] < 500))
    {
      Serial.println("Found Line! Leaving BWD Function");
      turn(!dir);
      break;
    }
    int positioning = qtrrc.readLine(sensorValues,1,1);                       // Get calibrated readings along with the line position
    int error = positioning - 1500;                                           // Determine the error from the calculated position

    int motorSpeed = KP * error + KD * (error - lastError);                // Adjust motorspeed according to constants KP and KD
    lastError = error;                                                     // Update last error to compare to next error

    rightMotorSpeed = BASESPEED + motorSpeed;                              // Update right and left motor speeds by the calculated motor speed
    leftMotorSpeed = BASESPEED - motorSpeed;

    if (rightMotorSpeed > MAXSPEED ) rightMotorSpeed = MAXSPEED ;          // Prevent the motor from going beyond max speed
    if (leftMotorSpeed > MAXSPEED ) leftMotorSpeed = MAXSPEED;             // Prevent the motor from going beyond max speed
    if (rightMotorSpeed < 0) rightMotorSpeed = 0;                          // Keep the motor speed positive
    if (leftMotorSpeed < 0) leftMotorSpeed = 0;                            // Keep the motor speed positive
  
    drive_motor(RIGHTMOTOR, BWD, rightMotorSpeed); 
    drive_motor(LEFTMOTOR, BWD, leftMotorSpeed);    
 }




/*********************** END OF PROGRAM ************************************************************************/    
}

