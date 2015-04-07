
/*
  This program uses PD control to follow a white line on a black background.
*/  

#include <QTRSensors.h>
#include <LiquidCrystal.h>
#include <Servo.h>

// define controller constants for error calculation
#define KP 1                      // Proportional Control Constant
#define KD 100                       // Derivative Control Constant. ( Note: KP < KD)

//define Max- and Basespeed
#define MAXSPEED 100          
#define BASESPEED 80 
#define TURNSPEED 80

//define line following set-up
#define NUM_SENSORS 6             // Number of sensors used to follow a straight line
#define NUM_POLLING_SENSORS 2     // Number of sensors used to poll for a 90 degree turn
#define NUM_TURNING_SENSORS 2     // Number of sensors used to stop turns
#define TIMEOUT       2500        // Waits for 2500 us for sensor outputs to go low. If they are not yet low, value is set at 2500
#define EMITTER_PIN   30           // Emitter is controlled by digital pin 30

//define photoresitor pin
#define PHOTORESITORPIN 0

//define communication pins
#define UNO_PIN_BOTTOM_OUT 47
#define UNO_PIN_BOTTOM_IN 44

#define UNO_PIN_TOP_OUT 46
#define UNO_PIN_TOP_IN 43


//define Front Gripper Pins
#define FGRIPPER1 42
#define FGRIPPER2 41

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

#define STOP 0
#define OPEN 1
#define CLOSE 2

// define threshold Values
#define THRESHOLD_LOW 500
#define THRESHOLD_HIGH 900

// sensor set-up according to QTR library
QTRSensorsRC qtrrc((unsigned char[]) {32, 33, 34, 35, 36, 37 } ,NUM_SENSORS, TIMEOUT, EMITTER_PIN);  // The 4 sensors used for following a straight line are digital pins 33, 34, 35, and 36
QTRSensorsRC poll((unsigned char[]) {38, 31} ,NUM_POLLING_SENSORS, TIMEOUT, EMITTER_PIN);    // The 2 polling sensors for 90 degree turns are digital pins 38 and 31
QTRSensorsRC turnIndicator((unsigned char[]) {39, 40} ,NUM_TURNING_SENSORS, TIMEOUT);        // The 2 polling sensors at the front are digital pins 39 and 40
unsigned int sensorValues[NUM_SENSORS];                                                      // An array containing the sensor values for the 4 line following sensors
unsigned int pollingValues[NUM_POLLING_SENSORS];                                             // An array containing the sensor values for the 2 polling sensors
unsigned int frontPollingValues[NUM_TURNING_SENSORS];                                        // An array containing the sensor values for the 2 front polling sensors

// debug lcd
LiquidCrystal lcd(49, 51, 53, 52, 50, 48);

Servo backGripper;
/********************  SETUP  ***************************************************************************************************************/

void setup()
{
  setupMotorshield();  // Jump to setupMotorshield to define pins as output
  backGripper.attach(4);
  backGripper.write(0);
  //start_course();
  auto_calibrate();   // function that calibrates the line following sensor
  front_gripper(OPEN);

  lcd.begin(16, 2);
  lcd.clear();
  
}
// Initialize error constant and motor speeds
int lastError = 0;      
int rightMotorSpeed = 0;
int leftMotorSpeed = 0;

//Declare global last turn variable
boolean lastTurn;
boolean lastGameTurn;
boolean gameTurn = 0;
boolean tIntersection = 0;
int turnCount = 0;
int gameCount = 0;




/******************   MAIN LOOP   ***************************************************************************************************************/

void loop()
{ 
  //check for upcoming turns
  poll.read(pollingValues);
  
  
  //attemted update of decisions
  if (pollingValues[0] <= THRESHOLD_LOW || pollingValues[1] <= THRESHOLD_LOW)
  {
    // read front sensor
    turnIndicator.read(frontPollingValues);
    
    // define sensor reader variables
    int FS = frontPollingValues[0] < frontPollingValues[1] ? frontPollingValues[0] : frontPollingValues[1];
    int RS = pollingValues[1];
    int LS = pollingValues[0];
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("turnCount:");
    lcd.setCursor(13,0);
    lcd.print(turnCount);
    lcd.setCursor(0,1);
    lcd.print(LS);
    lcd.setCursor(12,1);
    lcd.print(RS);
    lcd.setCursor(6,1);
    lcd.print(FS);
    stop_motors();
    
    // Check for Right Turn
    if (RS <= THRESHOLD_LOW && LS >= THRESHOLD_HIGH && FS >= THRESHOLD_HIGH)
    { 
      if (gameTurn == 1)
      {
        turnCount ++;
      }
      else if (turnCount > 0)
      {
        turnCount--;
      }
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Right Turn");
      turn(RIGHT);
    }
    // Check left turn
    else if (RS >= THRESHOLD_HIGH && LS <= THRESHOLD_LOW && FS >= THRESHOLD_HIGH)
    {
      if (gameTurn == 1)
      {
        turnCount ++;
      }
      else if (turnCount > 0)
      {
        turnCount--;
      }
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Left Turn");
      turn(LEFT);
    }
    // Check if gameturn that should be skipped
    else if ((RS >= THRESHOLD_HIGH || LS >= THRESHOLD_HIGH) && tIntersection == 1)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Skip Game Turn");
      drive_motor(RIGHT, FWD, BASESPEED);
      drive_motor(LEFT, FWD, BASESPEED);
      gameTurn = 0;
      tIntersection = 0;
      turnCount = 0;
      delay(200);
      
    }
     // Check right game turn
    else if (RS <= THRESHOLD_LOW && LS >= THRESHOLD_HIGH && FS <= THRESHOLD_HIGH && gameTurn == 0)
    {
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print("Right Game Turn");
       stop_motors();
       //delay(1000);
       lastGameTurn = RIGHT;
       gameTurn = 1;
       turnCount = 1;
       turn(RIGHT);
    }  
     // Check left game turn
    else if (RS >= THRESHOLD_HIGH && LS <= THRESHOLD_LOW && FS <= THRESHOLD_HIGH && gameTurn == 0)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Left Game Turn");
       stop_motors();
       //delay(1000);
       lastGameTurn = LEFT;
       gameTurn = 1;
       turnCount = 1;
       turn(LEFT);
    } 
     // Check for Game
    else if (FS <= THRESHOLD_LOW)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Play Game");
        stop_motors();
        gameCount++;
        play_game(gameCount);	
        gameTurn = 0;
    } 
    // T-intersection
    else 
    {
      
        if (turnCount == 1) 
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("T-int after game");
          turn(lastGameTurn);
          turnCount = 0;
        }
        else if (gameCount == 4)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("finish line");
          stop_motors();
          delay(10000);
        }
        else 
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("T-intersection");
          tIntersection = 1;
          gameTurn = 1;
          turnCount = 1;
      	  turn(RIGHT);
        }
    }
    lcd.setCursor(0,1);
    lcd.print(LS);
    lcd.setCursor(12,1);
    lcd.print(RS);
    lcd.setCursor(6,1);
    lcd.print(FS);
    stop_motors();
    //delay(5000);
  }
  

  // If there is no detected line on either polling sensor, continue with the PD Line Following
  { 
     
    int positioning = qtrrc.readLine(sensorValues,QTR_EMITTERS_ON, 1);                       // Get calibrated readings along with the line position
    int error = positioning - 2500;         // Determine the error from the calculated position
    
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

/************************** SETUP PINS ************************************************/
// This function sets up the pins
void setupMotorshield()
{
  
  // drive motors
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
  
  // front gripper 
  pinMode(FGRIPPER1, OUTPUT);
  pinMode(FGRIPPER2, OUTPUT);
  
  // communication pins
  
  pinMode(UNO_PIN_BOTTOM_OUT, OUTPUT);
  pinMode(UNO_PIN_BOTTOM_IN, INPUT);
  digitalWrite(UNO_PIN_BOTTOM_OUT, LOW);
  
  pinMode(UNO_PIN_TOP_OUT, OUTPUT);
  pinMode(UNO_PIN_TOP_IN, INPUT);
  digitalWrite(UNO_PIN_TOP_OUT, LOW);
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


/*********************** START OF THE GAME ************************************************************************/    

void start_course() 
{
  int currentRead =analogRead(PHOTORESITORPIN);
  while (currentRead < 500) 
  {
    currentRead = analogRead(PHOTORESITORPIN);
    delay(20);
  }
  poll.read(pollingValues);
  drive_motor(RIGHT, FWD, BASESPEED);
  drive_motor(LEFT, FWD, BASESPEED);
  delay(900);
  stop_motors();
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
  
  lastTurn = dir;
  if (dir == RIGHT)
  {
    drive_motor(RIGHTMOTOR, BWD, TURNSPEED);
    drive_motor(LEFTMOTOR, FWD, TURNSPEED);
    
    turnIndicator.read(frontPollingValues);
    while (frontPollingValues[1] < THRESHOLD_HIGH || frontPollingValues[0] < THRESHOLD_HIGH) 
    {
      turnIndicator.read(frontPollingValues);
      delay(15);
    }
    while (true)
    {
      turnIndicator.read(frontPollingValues);
      if (frontPollingValues[0] < THRESHOLD_LOW && frontPollingValues[1] < THRESHOLD_LOW)
      {
        delay(5);           
        stop_motors();
        break;
      }
    }
  }
  
  else if (dir == LEFT)
  {
    drive_motor(RIGHTMOTOR, FWD, TURNSPEED);
    drive_motor(LEFTMOTOR, BWD, TURNSPEED);
    
    turnIndicator.read(frontPollingValues);
    while (frontPollingValues[0] < THRESHOLD_HIGH || frontPollingValues[1] < THRESHOLD_HIGH) 
    {
      turnIndicator.read(frontPollingValues);
      delay(15);
    }
    while (true)
    {
      turnIndicator.read(frontPollingValues);
      if (frontPollingValues[0] < THRESHOLD_LOW && frontPollingValues[1] < THRESHOLD_LOW)
      {
        delay(5);                      
        stop_motors();
        break;
      }
    }
  }
 
  drive_motor(RIGHTMOTOR, FWD, BASESPEED);
  drive_motor(LEFTMOTOR, FWD, BASESPEED);
  delay(200);
  reset_motor_speeds();
}
  
  /************************** FOLLOW LINE BWDS ********************************************************************/
//function to follow line bwds after playing a game
void follow_bwd(boolean dir)
{
  while (pollingValues[0] < THRESHOLD_HIGH || pollingValues[1] < THRESHOLD_HIGH)
  {
    poll.read(pollingValues);
    
    drive_motor(RIGHTMOTOR, BWD, BASESPEED);
    drive_motor(LEFTMOTOR, BWD, BASESPEED);
  }
  reset_motor_speeds();

  while (true)
  {
    poll.read(pollingValues);
    if ((pollingValues[0] < THRESHOLD_LOW || pollingValues[1] < THRESHOLD_LOW))
    {
      turnCount--;
       
      if (turnCount == 0)
      {
        // reinitialize flags
        tIntersection = 0;
        gameTurn = 0;
        turn(!dir);
      }
      else 
      {
        turn(dir);
      }
      break;
    }
    int positioning = qtrrc.readLine(sensorValues,1,1);                       // Get calibrated readings along with the line position
    int error = positioning - 2500;                                           // Determine the error from the calculated position

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

}



/************************ FRONT GRIPPER **********************************************************************/

void front_gripper(int action)
{
  if(action == STOP)
  {
    digitalWrite(FGRIPPER1, LOW);
    digitalWrite(FGRIPPER2, LOW);
  }  
  
  else if(action == OPEN)
  {
    digitalWrite(FGRIPPER1, LOW);
    digitalWrite(FGRIPPER2, HIGH);
  }
  
  else if(action == CLOSE)
  {
    digitalWrite(FGRIPPER1, HIGH);
    digitalWrite(FGRIPPER2, LOW);
  }
  
  
}

/********************* FRONT GRIPPER ACTIONS ************************************************************************/ 

// grip game
void grip_game(int millisec)
{
  front_gripper(CLOSE);
  delay(millisec);
  
  front_gripper(OPEN);
  delay(500);
 
  drive_motor(RIGHT, FWD, 20); 
  drive_motor(LEFT, FWD, 20); 
  delay(500);
  
  stop_motors();
  
  front_gripper(CLOSE);  
}

// let go of game
void ungrip_game()
{
  front_gripper(OPEN);
  delay(200);
  
  drive_motor(RIGHT, BWD, 30); 
  drive_motor(LEFT, BWD, 30); 
  
  delay(500);
}


/********************* PLAY GAME ************************************************************************/    

void play_game(int gameCount) 
{
  switch(gameCount){
    case 1:
    {
      play_rubiks_cube();
      follow_bwd(lastTurn);
      break;
    }
    case 2:
    {
      play_etch_a_sketch();
      follow_bwd(lastTurn);
      break;
    }
    case 3:
    {
      // play rubics cube
      follow_bwd(lastTurn);
      break;
    }
    case 4:
    {
      pick_up_card();
      follow_bwd(lastTurn);
      break;
    }

  }
}


/********************* PLAY SIMON ************************************************************************/  

void play_simon()
{ 
  
  turnIndicator.read(frontPollingValues);
  drive_motor(RIGHT, FWD, 50);
  drive_motor(LEFT, BWD, 50);
  while (frontPollingValues[0] <= THRESHOLD_HIGH && frontPollingValues[1] <= THRESHOLD_HIGH) {
    turnIndicator.read(frontPollingValues);
    delay(20);
  }
  turnIndicator.read(frontPollingValues);
  while (frontPollingValues[0] >= THRESHOLD_LOW || frontPollingValues[1] >= THRESHOLD_LOW)
  {
    turnIndicator.read(frontPollingValues);
    delay(2);
  }
  
  drive_motor(RIGHT, BWD, 30);
  drive_motor(LEFT, BWD, 30);
  
  
  delay(1000);
  stop_motors();
  
  
  for (int i = 0; i < 180; i++) 
  {
    backGripper.write(i);
    delay(10);
  }
  delay(5000);
  
  for (int i = 180; i > 0; i--) 
  {
    backGripper.write(i);
    delay(5);
  }
  
  backGripper.detach();
  
  drive_motor(RIGHT, FWD, 30);
  drive_motor(LEFT, FWD, 30);
  
  poll.read(pollingValues);
  
  while (pollingValues[0] <= THRESHOLD_HIGH || pollingValues[1] <= THRESHOLD_HIGH)
  {
    poll.read(pollingValues);
    delay(20);
  }
  delay(100);
  
}

/********************* PLAY ETCH A SKETCH  ************************************************************************/  

void play_etch_a_sketch()
{
   
  // Grip the Game
  grip_game(3000);
  
  // sent signal to UNO
  digitalWrite(UNO_PIN_BOTTOM_OUT, HIGH);
  delay(1000);
  digitalWrite(UNO_PIN_BOTTOM_OUT, LOW);
  // wait for Uno to finish
  while (digitalRead(UNO_PIN_BOTTOM_IN) == LOW)
  {}
  
  ungrip_game();
  
}

/********************* PLAY RUBIKS CUBE  ************************************************************************/   
 
void play_rubiks_cube()
{
   // Grip the Game
  grip_game(5000);
  
  // sent signal to UNO
  digitalWrite(UNO_PIN_TOP_OUT, HIGH);
  delay(1000);
  digitalWrite(UNO_PIN_TOP_OUT, LOW);
  // wait for Uno to finish
  while (digitalRead(UNO_PIN_TOP_IN) == LOW)
  {}
  
  ungrip_game();
  
}
/********************* PICK UP CARD  ************************************************************************/  

void pick_up_card()
{
  
  // sent signal to UNO
  digitalWrite(UNO_PIN_BOTTOM_OUT, HIGH);
  delay(1000);
  digitalWrite(UNO_PIN_BOTTOM_OUT, LOW);
  // wait for Uno to finish
  while (digitalRead(UNO_PIN_BOTTOM_IN) == LOW)
  {}
  
}
/********************* END OF PROGRAM ************************************************************************/    


