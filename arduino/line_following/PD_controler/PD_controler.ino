/*
  This program uses PD control to follow a white line on a black background.
  The SeeedStudio Motorshield V2.2 on an Arduino UNO was used.
*/  

#include <QTRSensors.h>

#define Kp 1                     // Proportional Control Constant
#define Kd 200                     // Derivative Control Constant. ( Note: Kp < Kd)

#define rightMaxSpeed 110          // Max speed of the robot
#define leftMaxSpeed 110
#define rightBaseSpeed 60        // speed of the robot while traveling a straight line
#define leftBaseSpeed 60

#define NUM_SENSORS  4             // Number of sensors used to follow a straight line
#define NUM_POLLING_SENSORS 2      // Number of sensors used to poll for a 90 degree turn
#define TIMEOUT       2500         // Waits for 2500 us for sensor outputs to go low. If they are not yet low, value is set at 2500
#define EMITTER_PIN   1            // Emitter is controlled by digital pin 1

#define rightmotorForward 12        // Defined by the MotorShield. Do not use these pins for anything else.
#define rightmotorBackward 13       // MotorShield
#define rightMotorPWM 10            // MotorShield
#define leftmotorForward 8          // MotorShield
#define leftmotorBackward 11        // MotorShield
#define leftMotorPWM 9              // MotorShield

QTRSensorsRC qtrrc((unsigned char[]) { 3, 4, 5, 6} ,NUM_SENSORS, TIMEOUT, EMITTER_PIN);  // The 4 sensors used for following a straight line are digital pins 3, 4, 5, and 6
QTRSensorsRC poll((unsigned char[]) { 2, 7} ,NUM_POLLING_SENSORS, TIMEOUT, EMITTER_PIN); // The 2 polling sensors for 90 degree turns are digital pins 2 and 7
unsigned int sensorValues[NUM_SENSORS];                                                  // An array containing the sensor values for the 4 line following sensors
unsigned int pollingValues[NUM_POLLING_SENSORS];                                         // An array containing the sensor values for the 2 polling sensors
boolean polling[2];

/********************  SETUP  ***************************************************************************************************************/

void setup()
{
  setupMotorshield();                                         // Jump to setupMotorshield to define pins as output

  int i;
  for (int i = 0; i < 100; i++)
  {
    qtrrc.calibrate();
    delay(20);
  }  // Calibrate for sometime by sliding the sensors across the line.

  /* automatic calibration 
  if ( i  < 25 || i >= 75 )                                   // Turn to the left and right to expose the sensors to the brightest and darkest readings that may be encountered
     turn_right();  
  else
     turn_left(); */     
   delay(2000);                                               // Wait for 2s to position the bot before entering the main loop 
 } 

int lastError = 0;                                            // Initialize error constant and motor speeds
int rightMotorSpeed = 0;
int leftMotorSpeed = 0;

/******************   MAIN LOOP   ***************************************************************************************************************/

void loop()
{
  poll.read(pollingValues);
  for (unsigned char i = 0; i < (NUM_POLLING_SENSORS); i++)              // Read in the two polling sensors to see if one or the other detected a white line
  {
    if(pollingValues[i] <= 500) polling[i] = 1;                         // White
    else polling[i] = 0;                                                 // Black
  }
  
  if ((polling[0] == 1))                            // Check to see if there is a 90 degree turn to the right
  {
    analogWrite(leftMotorPWM, 0);                                        // Stop both motors for two seconds (for testing purposes to see if we detected the 90 degree turn)
    analogWrite(rightMotorPWM, 0);
    //delay(1000);
    //analogWrite(leftMotorPWM, 250);                                        // Keep right motor stopped, turn left motor for a set amount of time for a 90 degree turn
    delay(3000);                                                          // Adjust this value to get the 90 degree turn exact
  }
  if ((polling[1] == 1))                       // Check to see if there is a 90 degree turn to the left
  {
    analogWrite(leftMotorPWM, 0);                                        // Stop both motors for two seconds (for testing purposes to see if we detected the 90 degree turn)
    analogWrite(rightMotorPWM, 0);
    //delay(1000);
    //analogWrite(rightMotorPWM, 250);                                        // Keep right motor stopped, turn left motor for a set amount of time for a 90 degree turn
    delay(3000);                                                          // Adjust this value to get the 90 degree turn exact
  }
  else                                                                   // If there is no detected line on either polling sensor, continue with the PD Line Following
  {  
    int position = qtrrc.readLine(sensorValues,1,1);                            // Get calibrated readings along with the line position
    int error = position - 1500;                                           // Determine the error from the calculated position
    
    int motorSpeed = Kp * error + Kd * (error - lastError);                // Adjust motorspeed according to constants Kp and Kd
    lastError = error;                                                     // Update last error to compare to next error
  
    rightMotorSpeed = rightBaseSpeed + motorSpeed/10;                         // Update right and left motor speeds by the calculated motor speed
    leftMotorSpeed = leftBaseSpeed - motorSpeed/10;
    
    if (rightMotorSpeed > rightMaxSpeed ) rightMotorSpeed = rightMaxSpeed;   // Prevent the motor from going beyond max speed
    if (leftMotorSpeed > leftMaxSpeed ) leftMotorSpeed = leftMaxSpeed;       // Prevent the motor from going beyond max speed
    if (rightMotorSpeed < 0) rightMotorSpeed = 0;                            // Keep the motor speed positive
    if (leftMotorSpeed < 0) leftMotorSpeed = 0;                              // Keep the motor speed positive
  }
  
  {
    analogWrite(rightMotorPWM, rightMotorSpeed);                            // Send the new calculated motor speeds from the motor controller to the motors
    analogWrite(leftMotorPWM, leftMotorSpeed);
  }
}
/**********************************************************************************/
void setupMotorshield()
{
  pinMode(rightmotorForward, OUTPUT);
  pinMode(rightmotorBackward, OUTPUT);
  pinMode(rightMotorPWM, OUTPUT);
  pinMode(leftmotorForward, OUTPUT);
  pinMode(leftmotorBackward, OUTPUT);
  pinMode(leftMotorPWM, OUTPUT);
  
  digitalWrite(rightmotorForward, HIGH);
  digitalWrite(rightmotorBackward, LOW);
  digitalWrite(leftmotorForward, HIGH);
  digitalWrite(leftmotorBackward, LOW);
}


