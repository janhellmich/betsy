#include <Servo.h>

Servo Servo_1;
Servo Servo_2;
Servo Servo_start;
int pos_1 = 0;
int pos_2 = 0;
int pos_start = 0;
int Press = 0;


const int Sensor_Red = 0;    //analog pin assignments
const int Sensor_Green = 1;   
const int Sensor_Yellow = 2; 
const int Sensor_Blue = 3;  

int Brightness_Red = 0;        
int Brightness_Green = 0;        
int Brightness_Yellow = 0;        
int Brightness_Blue = 0;

int Red_Sample_1 = 0;        //Ambient light samples for Setpoints
int Red_Sample_2 = 0;
int Red_Sample_3 = 0;
int Red_Sample_4 = 0;
int Red_Sample_5 = 0;
int Red_Setpoint = 0;

int Green_Sample_1 = 0;
int Green_Sample_2 = 0;
int Green_Sample_3 = 0;
int Green_Sample_4 = 0;
int Green_Sample_5 = 0;
int Green_Setpoint = 0;

int Yellow_Sample_1 = 0;
int Yellow_Sample_2 = 0;
int Yellow_Sample_3 = 0;
int Yellow_Sample_4 = 0;
int Yellow_Sample_5 = 0;
int Yellow_Setpoint = 0;

int Blue_Sample_1 = 0;
int Blue_Sample_2 = 0;
int Blue_Sample_3 = 0;
int Blue_Sample_4 = 0;
int Blue_Sample_5 = 0;
int Blue_Setpoint = 0;

void setup()
{
  Serial.begin(9600);
  
  pinMode(Sensor_Red, INPUT);
  pinMode(Sensor_Green, INPUT);
  pinMode(Sensor_Yellow, INPUT);
  pinMode(Sensor_Blue, INPUT);
    
  Servo_1.attach(10);            //PWM pin assignments
  Servo_2.attach(11);
  Servo_start.attach(9);
  
  Servo_1.write(35);              //Servo Starting Angles
  Servo_2.write(35);
  Servo_start.write(30);
}

void loop()
{
  Red_Sample_1 = analogRead(Sensor_Red);          //Take 5 Samples of ambient light for each of the 4 sensors
  Green_Sample_1 = analogRead(Sensor_Green);
  Yellow_Sample_1 = analogRead(Sensor_Yellow);
  Blue_Sample_1 = analogRead(Sensor_Blue);
  delay(200);
  Red_Sample_2 = analogRead(Sensor_Red);
  Green_Sample_2 = analogRead(Sensor_Green);
  Yellow_Sample_2 = analogRead(Sensor_Yellow);
  Blue_Sample_2 = analogRead(Sensor_Blue);
  delay(200);
  Red_Sample_3 = analogRead(Sensor_Red);
  Green_Sample_3 = analogRead(Sensor_Green);
  Yellow_Sample_3 = analogRead(Sensor_Yellow);
  Blue_Sample_3 = analogRead(Sensor_Blue);
  delay(200);
  Red_Sample_4 = analogRead(Sensor_Red);
  Green_Sample_4 = analogRead(Sensor_Green);
  Yellow_Sample_4 = analogRead(Sensor_Yellow);
  Blue_Sample_4 = analogRead(Sensor_Blue);
  delay(200);
  Red_Sample_5 = analogRead(Sensor_Red);
  Green_Sample_5 = analogRead(Sensor_Green);
  Yellow_Sample_5 = analogRead(Sensor_Yellow);
  Blue_Sample_5 = analogRead(Sensor_Blue);
  delay(200);
  
  Red_Setpoint = ((Red_Sample_1 + Red_Sample_2 + Red_Sample_3 + Red_Sample_4 + Red_Sample_5)/5)+30;                        //Take average of 5 ambient light settings and add a margin for error. This is now the setpoint
  Serial.println(Red_Setpoint);
  Green_Setpoint = ((Green_Sample_1 + Green_Sample_2 + Green_Sample_3 + Green_Sample_4 + Green_Sample_5)/5)+30;              //This can be done more times throughout program for better reliability
  Serial.println(Green_Setpoint);
  Yellow_Setpoint = ((Yellow_Sample_1 + Yellow_Sample_2 + Yellow_Sample_3 + Yellow_Sample_4 + Yellow_Sample_5)/5)+40;
  Serial.println(Yellow_Setpoint);
  Blue_Setpoint = ((Blue_Sample_1 + Blue_Sample_2 + Blue_Sample_3 + Blue_Sample_4 + Blue_Sample_5)/5)+30;
  Serial.println(Blue_Setpoint);
  
  delay(200);
  
   for(pos_start = 30; pos_start >= 1; pos_start -= 1)                                          //Press Center Button to start game
    {
      Servo_start.write(pos_start);
      delay(15);
    }
      Brightness_Red = analogRead(Sensor_Red);                                        //Take all sensor readings
      Brightness_Green = analogRead(Sensor_Green);
      Brightness_Yellow = analogRead(Sensor_Yellow);
      Brightness_Blue = analogRead(Sensor_Blue);
    for(pos_start = 1; pos_start <= 30; pos_start += 1)                               //Release Start Button
    {
      Servo_start.write(pos_start);
      delay(15);
    }
 
  Compare_Brightness(pos_1, pos_2);                                                   //Go to Function Compare_Brightness_1 to determine which color is lit and which button to press
  
  delay(725);                                                                         //Delay a certain amount of time to wait for 2nd color to illuminate
  Press = 0;                                                                          //Reinitialize Press to zero for second reading
  Brightness_Red = analogRead(Sensor_Red);                                                          //Read 2nd color
  Brightness_Green = analogRead(Sensor_Green);
  Brightness_Yellow = analogRead(Sensor_Yellow);
  Brightness_Blue = analogRead(Sensor_Blue);
  Compare_Brightness(pos_1, pos_2);                                                                  //Go to Function Compare_Brightness_2 to determine which color is lit and which button to press
  
  delay(1260);                                                                                      //Delay a certain amount of time to wait for 3rd color to illuminate
  Press = 0;
  Brightness_Red = analogRead(Sensor_Red);                                                          //Read 3rd color
  Brightness_Green = analogRead(Sensor_Green);
  Brightness_Yellow = analogRead(Sensor_Yellow);
  Brightness_Blue = analogRead(Sensor_Blue);
  Compare_Brightness(pos_1, pos_2);                                                              //Go to Function Compare_Brightness_3 to determine which color is lit and which button to press
  
  while(1) {}                                                                                      //Continuous loop (My way of ending the program)

}


/*********************************************************/
/**********************FUNCTIONS**************************/
/*********************************************************/

int Compare_Brightness(int pos_1, int pos_2)                                                //Determine which color is the 1st illuminated
{
  Serial.println(Brightness_Red);  
  if (Brightness_Red > Red_Setpoint)                                                          //If Red sensor shows brightness > Red Setpoint, Store Value of 1 in variable Press_1
  {
    Press = 1;
  }
  /*******************************************************/
  
  Serial.println(Brightness_Green);
  if (Brightness_Green > Green_Setpoint)                                                      //If Green sensor shows brightness > Green Setpoint, Store Value of 2 in variable Press_1
  {
    Press = 2;                                                                              
  }
  /*******************************************************/
  
 Serial.println(Brightness_Yellow);
  if (Brightness_Yellow >Yellow_Setpoint)                                                    //If Yellow sensor shows brightness > Yellow Setpoint, Store Value of 3 in variable Press_1
  {
    Press = 3;
  }
  /******************************************************/
  
  Serial.println(Brightness_Blue);
  if (Brightness_Blue > Blue_Setpoint)                                                      //If Blue sensor shows brightness > Blue Setpoint, Store Value of 4 in variable Press_1
  {
    Press = 4;
  }
  Serial.println(Press);
  /********************************************************/
  delay(1000);
  if (Press == 1)                                                                          //If Press_1 = 1, Red is on
  {
    for(pos_1 = 35; pos_1 >=1 ; pos_1 -= 1)                                                  //Press Red
    {
      Servo_1.write(pos_1);
      delay(15);
    }
    for(pos_1 = 1; pos_1 <= 35; pos_1 += 1)
    {
      Servo_1.write(pos_1);
      delay(15);
    }
  }
  
  if (Press == 2)                                                                          //If Press_1 = 2, Green is on
  {
    for(pos_1 = 35; pos_1 <= 70; pos_1 += 1)                                                  //Press Green
    {
      Servo_1.write(pos_1);
      delay(15);
    }
    for(pos_1 = 70; pos_1 >= 35; pos_1 -= 1)
    {
      Servo_1.write(pos_1);
      delay(15);
    }
  }
  
  if (Press == 3)                                                                        //If Press_1 = 3, Yellow is on
  {
    for(pos_2 = 35; pos_2 >= 1; pos_2 -= 1)                                                  //Press Yellow
    {
      Servo_2.write(pos_2);
      delay(15);
    }
    for(pos_2 = 1; pos_2 <= 35; pos_2 += 1)
    {
      Servo_2.write(pos_2);
      delay(15);
    }
  }
  if (Press == 4)                                                                        //If Press_1 = 4, Blue is on
  {
    for(pos_2 = 35; pos_2 <= 70; pos_2 += 1)                                                  //Press Blue
    {
      Servo_2.write(pos_2);
      delay(15);
    }
    for(pos_2 = 70; pos_2 >= 35; pos_2 -= 1)
    {
      Servo_2.write(pos_2);
      delay(15);
    } 
  }
} 

