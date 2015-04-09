
/*
  This program uses PD control to follow a white line on a black background.
*/  

#include <QTRSensors.h>
#include <LiquidCrystal.h>
#include <Servo.h>

// define controller constants for error calculation
#define KP 1                      // Proportional Control Constant
#define KD 100                       // Derivative Control Constant. ( Note: KP < KD)

//define Max- and BASE_SPEED

#define MAX_SPEED 100          
#define BASE_SPEED 80 
#define TURN_SPEED 80

//define line following set-up
#define NUM_SENSORS 6             // Number of sensors used to follow a straight line
#define NUM_POLLING_SENSORS 2     // Number of sensors used to poll for a 90 degree turn
#define NUM_TURNING_SENSORS 2     // Number of sensors used to stop turns
#define TIMEOUT       2500        // Waits for 2500 us for sensor outputs to go low. If they are not yet low, value is set at 2500
#define EMITTER_PIN   30           // Emitter is controlled by digital pin 30
#define START_LED_PIN 45

//define photoresitor pin
#define PHOTORESISTOR_PIN 0

// T-intersection pins
#define T_INT_1 10
#define T_INT_2 11
#define T_INT_3 12
#define T_INT_4 13

//define communication pins
#define UNO_PIN_BOTTOM_OUT 47
#define UNO_PIN_BOTTOM_IN 46

#define UNO_PIN_TOP_OUT 9
#define UNO_PIN_TOP_IN 8


//define Front Gripper Pins
#define F_GRIPPER_1 42
#define F_GRIPPER_2 41

//define constants for motor shield pin assignments
#define STBY 24
#define RIGHT_MOTOR_FORWARD 23        
#define RIGHT_MOTOR_BACKWARD 22       
#define RIGHT_MOTOR_PWM 2             
#define LEFT_MOTOR_FORWARD 25         
#define LEFT_MOTOR_BACKWARD 26        
#define LEFT_MOTOR_PWM 3              

//define constants for motor control
#define RIGHT_MOTOR 1
#define LEFT_MOTOR 0
#define FWD 1
#define BWD 0
#define RIGHT 1
#define LEFT 0

#define STOP 0
#define OPEN 1
#define CLOSE 2

// define threshold Values
#define THRESHOLD_LOW 500
#define THRESHOLD_HIGH 1100



// sensor set-up accordingto QTR library
QTRSensorsRC qtrrc((unsigned char[]) {32, 33, 34, 35, 36, 37 } ,NUM_SENSORS, TIMEOUT, EMITTER_PIN);  // The 4 sensors used for following a straight line are digital pins 33, 34, 35, and 36
QTRSensorsRC poll((unsigned char[]) {38, 31} ,NUM_POLLING_SENSORS, TIMEOUT, EMITTER_PIN);    // The 2 polling sensors for 90 degree turns are digital pins 38 and 31
QTRSensorsRC turnIndicator((unsigned char[]) {39, 40} ,NUM_TURNING_SENSORS, TIMEOUT);        // The 2 polling sensors at the front are digital pins 39 and 40
unsigned int sensorValues[NUM_SENSORS];                                                      // An array containing the sensor values for the 4 line following sensors
unsigned int pollingValues[NUM_POLLING_SENSORS];                                             // An array containing the sensor values for the 2 polling sensors
unsigned int frontPollingValues[NUM_TURNING_SENSORS];                                        // An array containing the sensor values for the 2 front polling sensors




// debug lcd
LiquidCrystal lcd(49, 51, 53, 52, 50, 48);


Servo backGripper;


int tInt [4];



/********************  SETUP  ***************************************************************************************************************/


void setup()

{
  
  // DEBUG
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();
  
  
  setupMotorshield();
  
  backGripper.attach(4);
  backGripper.write(0);
  
  // DEBUG
  if (digitalRead(T_INT_4))
  {
    start_course();
  }
  
  auto_calibrate();   
 
  front_gripper(OPEN);
  
  get_t_intersections();
 
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
int tIntCount = 0;




/******************   MAIN LOOP   ***************************************************************************************************************/

void loop()

{ 
  
  // read polling sensors
  poll.read(pollingValues);
  
  // check if decisions has to be made
  if (pollingValues[0] <= THRESHOLD_LOW || pollingValues[1] <= THRESHOLD_LOW)
  
  {
    
    // read front sensor
    turnIndicator.read(frontPollingValues);
    
    // define sensor reader variables
    int FS = frontPollingValues[0] < frontPollingValues[1] ? frontPollingValues[0] : frontPollingValues[1];
    int RS = pollingValues[1];
    int LS = pollingValues[0];
    
    
    // DEBUG
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
      
      // DEBUG
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
      
      // DEBUG
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Left Turn");
      
      
      turn(LEFT);
      
    }
    
    // Check if gameturn that should be skipped
    else if ((RS >= THRESHOLD_HIGH || LS >= THRESHOLD_HIGH) && tIntersection == 1)
    
    {
      
      // DEBUG
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Skip Game Turn");
      
      
      drive_motor(RIGHT, FWD, BASE_SPEED);
      drive_motor(LEFT, FWD, BASE_SPEED);
      gameTurn = 0;
      tIntersection = 0;
      turnCount = 0;
      delay(200);
      
    }
    
     // Check right game turn
    else if (RS <= THRESHOLD_LOW && LS >= THRESHOLD_HIGH && FS <= THRESHOLD_HIGH && gameTurn == 0)
    
    {
       // DEBUG
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
      
      //DEBUG
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Left Game Turn");
      
      
       stop_motors();
       lastGameTurn = LEFT;
       gameTurn = 1;
       turnCount = 1;
       turn(LEFT);
       
    } 
    
     // Check for Game
    else if (FS <= THRESHOLD_LOW)
    
    {
      
        // DEBUG
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
          
          // DEBUG
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("T-int after game");
          
          turn(lastGameTurn);
          turnCount = 0;
          
        }
        
        else if (gameCount == 4)
        {
          
          // DEBUG
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("finish line");
          
          stop_motors();
          delay(10000);
          
        }
        
        else 
        {
          
          //DEBUG
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("T-intersection");
          
          tIntersection = 1;
          gameTurn = 1;
          turnCount = 1;
      	  turn(tInt[tIntCount]);
          tIntCount++;
          
        }
        
    }
    
    // DEBUG
    lcd.setCursor(0,1);
    lcd.print(LS);
    lcd.setCursor(12,1);
    lcd.print(RS);
    lcd.setCursor(6,1);
    lcd.print(FS);
    stop_motors();
    //delay(10000);
    
  }
  

  // If there is no detected line on either polling sensor, continue with the PD Line Following
  { 
     
    int positioning = qtrrc.readLine(sensorValues,QTR_EMITTERS_ON, 1);                       // Get calibrated readings along with the line position
    int error = positioning - 2500;         // Determine the error from the calculated position
    
    int motorSpeed = KP * error + KD * (error - lastError);                // Adjust motorspeed according to constants KP and KD
    lastError = error;                                                     // Update last error to compare to next error
  
    rightMotorSpeed = BASE_SPEED + motorSpeed;                              // Update right and left motor speeds by the calculated motor speed
    leftMotorSpeed = BASE_SPEED - motorSpeed;
    
    if (rightMotorSpeed > MAX_SPEED ) rightMotorSpeed = MAX_SPEED ;          // Prevent the motor from going beyond max speed
    if (leftMotorSpeed > MAX_SPEED ) leftMotorSpeed = MAX_SPEED;             // Prevent the motor from going beyond max speed
    if (rightMotorSpeed < 0) rightMotorSpeed = 0;                          // Keep the motor speed positive
    if (leftMotorSpeed < 0) leftMotorSpeed = 0;                            // Keep the motor speed positive                           
  }
  
  {
    // Use drive function with corrected motor speeds
    drive_motor(RIGHT_MOTOR, FWD, rightMotorSpeed); 
    drive_motor(LEFT_MOTOR, FWD, leftMotorSpeed);    
  }
  
}


/************************** FUNCTIONS ********************************************************/

/************************** SETUP PINS ************************************************/
// This function sets up the pins
void setupMotorshield()

{
  
  // drive motors
  pinMode(STBY, OUTPUT);
  pinMode(RIGHT_MOTOR_FORWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_BACKWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM, OUTPUT);
  pinMode(LEFT_MOTOR_FORWARD, OUTPUT);
  pinMode(LEFT_MOTOR_BACKWARD, OUTPUT);
  pinMode(LEFT_MOTOR_PWM, OUTPUT);
  
  digitalWrite(STBY, HIGH);
  digitalWrite(RIGHT_MOTOR_FORWARD, HIGH);
  digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
  digitalWrite(LEFT_MOTOR_FORWARD, HIGH);
  digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
  
  
  // front gripper 
  pinMode(F_GRIPPER_1, OUTPUT);
  pinMode(F_GRIPPER_2, OUTPUT);
  
  
  //Start LED
  pinMode(START_LED_PIN, OUTPUT);
  
  
  //T-intersections
  pinMode(T_INT_1, INPUT);
  pinMode(T_INT_2, INPUT);
  pinMode(T_INT_3, INPUT);
  pinMode(T_INT_4, INPUT);
  
  
  // communication pins
  pinMode(UNO_PIN_BOTTOM_OUT, OUTPUT);
  pinMode(UNO_PIN_BOTTOM_IN, INPUT);
  digitalWrite(UNO_PIN_BOTTOM_OUT, LOW);
  
  
  pinMode(UNO_PIN_TOP_OUT, OUTPUT);
  pinMode(UNO_PIN_TOP_IN, INPUT);
  digitalWrite(UNO_PIN_TOP_OUT, LOW);
  
}

/************************** GET THE T-INTERSECTIONS ************************************************/

void get_t_intersections()

{
 
    tInt[0] = digitalRead(T_INT_1);
    Serial.print(tInt[0]);
    tInt[1] = digitalRead(T_INT_2);
    Serial.print(tInt[1]);
    tInt[2] = digitalRead(T_INT_3);
    Serial.print(tInt[2]);
    tInt[3] = digitalRead(T_INT_4);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(tInt[0]);
    lcd.setCursor(2,0);
    lcd.print(tInt[1]);
    lcd.setCursor(4,0);
    lcd.print(tInt[2]);
    lcd.setCursor(6,0);
    lcd.print(tInt[3]);
    Serial.print(tInt[3]);
    Serial.print('\n');
    delay(500);
  
  
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
  
  int currentRead =analogRead(PHOTORESISTOR_PIN);
  
  while (currentRead < 20) 
  {
    
    currentRead = analogRead(PHOTORESISTOR_PIN);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(currentRead);
    delay(20);
    
  }
  
  digitalWrite(START_LED_PIN, HIGH);
  
  while (currentRead > 18) 
  {
    
    currentRead = analogRead(PHOTORESISTOR_PIN);
    delay(20);
    
  }

  drive_motor(RIGHT, FWD, BASE_SPEED);
  drive_motor(LEFT, FWD, BASE_SPEED);
  delay(900);
  stop_motors();
  
}

/************************** DRIVE MOTORS **********************************************************************/

// this function drives the motors: specify motor, direction, speed
void drive_motor(boolean motor, boolean dir, int spd)

{
  
  if (motor == RIGHT_MOTOR)
  {
    
    if (dir == FWD) //Right motor forward
    {
      
      digitalWrite(RIGHT_MOTOR_FORWARD, HIGH);
      digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
      analogWrite(RIGHT_MOTOR_PWM, spd);                            // Send the new calculated motor speeds from the motor controller to the motors
      
    }
    
    else if(dir == BWD)//Right motor backwards
    {
      
      digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
      digitalWrite(RIGHT_MOTOR_BACKWARD, HIGH);
      analogWrite(RIGHT_MOTOR_PWM, spd);                            // Send the new calculated motor speeds from the motor controller to the motors
    
    }
    
  }
  
  else if(motor == LEFT_MOTOR)
  {
    
    if (dir == FWD)//Left motor forwards
    {
      
      digitalWrite(LEFT_MOTOR_FORWARD, HIGH);
      digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
      analogWrite(LEFT_MOTOR_PWM, spd);                            // Send the new calculated motor speeds from the motor controller to the motors
      
    }
    else if(dir == BWD)//Right motor backwards
    {
      digitalWrite(LEFT_MOTOR_FORWARD, LOW);
      digitalWrite(LEFT_MOTOR_BACKWARD, HIGH);
      analogWrite(LEFT_MOTOR_PWM, spd);      // Send the new calculated motor speeds from the motor controller to the motors
    
    }
    
  }  
  
}

/************************* STOP MOTORS ***************************************************************/
// stop both motors
void stop_motors()
{
  
  drive_motor(RIGHT_MOTOR, FWD, 0);
  drive_motor(LEFT_MOTOR, FWD, 0);
  
}

/************************** RESET MOTOR SPEEDS ********************************************************************/
// reset motor speeds
void reset_motor_speeds()
{
  
  rightMotorSpeed = BASE_SPEED;
  leftMotorSpeed = BASE_SPEED;
  
}

/************************** TURNS ********************************************************************/
// initiate a turn: specify direction
void turn(boolean dir)

{
  
  lastTurn = dir;
  
  if (dir == RIGHT)
  {
    
    drive_motor(RIGHT_MOTOR, BWD, TURN_SPEED);
    drive_motor(LEFT_MOTOR, FWD, TURN_SPEED);
    
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
    
    drive_motor(RIGHT_MOTOR, FWD, TURN_SPEED);
    drive_motor(LEFT_MOTOR, BWD, TURN_SPEED);
    
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
 
  drive_motor(RIGHT_MOTOR, FWD, BASE_SPEED);
  drive_motor(LEFT_MOTOR, FWD, BASE_SPEED);
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
    
    drive_motor(RIGHT_MOTOR, BWD, BASE_SPEED);
    drive_motor(LEFT_MOTOR, BWD, BASE_SPEED);
    
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

    rightMotorSpeed = BASE_SPEED + motorSpeed;                              // Update right and left motor speeds by the calculated motor speed
    leftMotorSpeed = BASE_SPEED - motorSpeed;

    if (rightMotorSpeed > MAX_SPEED ) rightMotorSpeed = MAX_SPEED ;          // Prevent the motor from going beyond max speed
    if (leftMotorSpeed > MAX_SPEED ) leftMotorSpeed = MAX_SPEED;             // Prevent the motor from going beyond max speed
    if (rightMotorSpeed < 0) rightMotorSpeed = 0;                          // Keep the motor speed positive
    if (leftMotorSpeed < 0) leftMotorSpeed = 0;                            // Keep the motor speed positive
  
    drive_motor(RIGHT_MOTOR, BWD, rightMotorSpeed); 
    drive_motor(LEFT_MOTOR, BWD, leftMotorSpeed);    
    
  }

}



/************************ FRONT GRIPPER **********************************************************************/

void front_gripper(int action)
{
  
  if(action == STOP)
  {
    
    digitalWrite(F_GRIPPER_1, LOW);
    digitalWrite(F_GRIPPER_2, LOW);
    
  }  
  
  else if(action == OPEN)
  {
    
    digitalWrite(F_GRIPPER_1, LOW);
    digitalWrite(F_GRIPPER_2, HIGH);
    
  }
  
  else if(action == CLOSE)
  {
    
    digitalWrite(F_GRIPPER_1, HIGH);
    digitalWrite(F_GRIPPER_2, LOW);
    
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
  delay(1000);
  
  stop_motors();
  
  front_gripper(CLOSE); 
  
}

// let go of game
void ungrip_game()
{
  
  front_gripper(OPEN);
  delay(50);
  
  drive_motor(RIGHT, BWD, 20); 
  drive_motor(LEFT, BWD, 20); 
  delay(1000);
  
}


/********************* PLAY GAME ************************************************************************/    

void play_game(int gameCount) 
{
  
  switch(gameCount){
    case 1:
    {
      play_simon();
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
      
      play_rubiks_cube();
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
  
  // back up
  drive_motor(RIGHT, BWD, 30);
  drive_motor(LEFT, BWD, 30);
  delay(500);
  
  // start turning
  drive_motor(RIGHT, FWD, 50);
  drive_motor(LEFT, BWD, 50);
  
  turnIndicator.read(frontPollingValues);
  
  // pass initial white line
  while (frontPollingValues[0] <= THRESHOLD_HIGH && frontPollingValues[1] <= THRESHOLD_HIGH) 
  {
    
    turnIndicator.read(frontPollingValues);
    delay(20);
    
  }
  
  turnIndicator.read(frontPollingValues);
  
  // wait for next white line
  while (frontPollingValues[0] >= THRESHOLD_LOW || frontPollingValues[1] >= THRESHOLD_LOW)
  {
    turnIndicator.read(frontPollingValues);
    delay(2);
  }
  
  // approach game
  drive_motor(RIGHT, BWD, 30);
  drive_motor(LEFT, BWD, 30);
  delay(1300);
  
  stop_motors();
  
  // grip game
  for (int i = 0; i < 80; i++) 
  {
    backGripper.write(i);
    delay(10);
  }
  
  for (int i = 80; i > 50; i--) 
  {
    backGripper.write(i);
    delay(5);
  }
  
  drive_motor(RIGHT, BWD, 25);
  drive_motor(LEFT, BWD, 25);
  
  delay(550);
  stop_motors();
  
  for (int i = 50; i < 80; i++) 
  {
    backGripper.write(i);
    delay(10);
  }
  
  // sent signal to UNO
  digitalWrite(UNO_PIN_TOP_OUT, HIGH);
  delay(1000);
  digitalWrite(UNO_PIN_TOP_OUT, LOW);
  
  
  // wait for Uno to finish
  while (digitalRead(UNO_PIN_TOP_IN) == LOW)
  {}
  
  for (int i = 80; i > 0; i--) 
  {
    backGripper.write(i);
    delay(5);
  }
  
  backGripper.detach();
  
  drive_motor(RIGHT, FWD, 30);
  drive_motor(LEFT, FWD, 30);
  
  poll.read(pollingValues);
  
  while (pollingValues[0] <= THRESHOLD_HIGH && pollingValues[1] <= THRESHOLD_HIGH)
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


