#include <Servo.h>

Servo Servo_1;
Servo Servo_2;
Servo Servo_start;

Servo arm;

int pos_1 = 0;
int pos_2 = 0;
int pos_start = 0;
int Press_1 = 0;
int Press_2 = 0;
int Press_3 = 0;

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
  
  arm.attach(5);
  arm.write(90);
  delay(2000);
  arm.write(180);
  
  
  pinMode(Sensor_Red, INPUT);
  pinMode(Sensor_Green, INPUT);
  pinMode(Sensor_Yellow, INPUT);
  pinMode(Sensor_Blue, INPUT);
    
  Servo_1.attach(9);            //PWM pin assignments
  Servo_2.attach(10);
  Servo_start.attach(11);
  
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
      Brightness_Red = analogRead(Sensor_Red);                                                    //Take all sensor readings
      Brightness_Green = analogRead(Sensor_Green);
      Brightness_Yellow = analogRead(Sensor_Yellow);
      Brightness_Blue = analogRead(Sensor_Blue);
    for(pos_start = 1; pos_start <= 30; pos_start += 1)                                            //Release Start Button
    {
      Servo_start.write(pos_start);
      delay(15);
    }
 
  Compare_Brightness_1(pos_1, pos_2);                                                              //Go to Function Compare_Brightness_1 to determine which color is lit and which button to press
  
  delay(725);                                                                                      //Delay a certain amount of time to wait for 2nd color to illuminate
  Brightness_Red = analogRead(Sensor_Red);                                                          //Read 2nd color
  Brightness_Green = analogRead(Sensor_Green);
  Brightness_Yellow = analogRead(Sensor_Yellow);
  Brightness_Blue = analogRead(Sensor_Blue);
  Compare_Brightness_2(pos_1, pos_2);                                                                  //Go to Function Compare_Brightness_2 to determine which color is lit and which button to press
  
  delay(1260);                                                                                      //Delay a certain amount of time to wait for 3rd color to illuminate
  Brightness_Red = analogRead(Sensor_Red);                                                          //Read 3rd color
  Brightness_Green = analogRead(Sensor_Green);
  Brightness_Yellow = analogRead(Sensor_Yellow);
  Brightness_Blue = analogRead(Sensor_Blue);
  Compare_Brightness_3(pos_1, pos_2);                                                              //Go to Function Compare_Brightness_3 to determine which color is lit and which button to press
  
  while(1) {}                                                                                      //Continuous loop (My way of ending the program)

}


/*********************************************************/
/**********************FUNCTIONS**************************/
/*********************************************************/

int Compare_Brightness_1(int pos_1, int pos_2)                                                //Determine which color is the 1st illuminated
{
  Serial.println(Brightness_Red);  
  if (Brightness_Red > Red_Setpoint)                                                          //If Red sensor shows brightness > Red Setpoint, Store Value of 1 in variable Press_1
  {
    Press_1 = 1;
  }
  /*******************************************************/
  
  Serial.println(Brightness_Green);
  if (Brightness_Green > Green_Setpoint)                                                      //If Red sensor shows brightness > Red Setpoint, Store Value of 2 in variable Press_1
  {
    Press_1 = 2;                                                                              
  }
  /*******************************************************/
  
 Serial.println(Brightness_Yellow);
  if (Brightness_Yellow >Yellow_Setpoint)                                                    //If Yellow sensor shows brightness > Yellow Setpoint, Store Value of 3 in variable Press_1
  {
    Press_1 = 3;
  }
  /******************************************************/
  
  Serial.println(Brightness_Blue);
  if (Brightness_Blue > Blue_Setpoint)                                                      //If Blue sensor shows brightness > Blue Setpoint, Store Value of 4 in variable Press_1
  {
    Press_1 = 4;
  }
  Serial.println(Press_1);
  /********************************************************/
  delay(1000);
  if (Press_1 == 1)                                                                          //If Press_1 = 1, Red is on
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
  
  if (Press_1 == 2)                                                                          //If Press_1 = 2, Green is on
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
  
  if (Press_1 == 3)                                                                        //If Press_1 = 3, Yellow is on
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
  if (Press_1 == 4)                                                                        //If Press_1 = 4, Blue is on
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
/*********************************************************************************************/

int Compare_Brightness_2(int pos_1, int pos_2)                                              //Determine which color is the 2nd illuminated
{
  Serial.println(Brightness_Red);  
  if (Brightness_Red > Red_Setpoint)                                                        //If Red sensor shows brightness > Red Setpoint, Store Value of 1 in variable Press_2
  {
    Press_2 = 1;
  }
  /*******************************************************/
  
  Serial.println(Brightness_Green);
  if (Brightness_Green > Green_Setpoint)                                                    //If Green sensor shows brightness > Green Setpoint, Store Value of 2 in variable Press_2
  {
    Press_2 = 2;
  }
  /*******************************************************/
  
 Serial.println(Brightness_Yellow);
  if (Brightness_Yellow >Yellow_Setpoint)                                                      //If Yellow sensor shows brightness > Yellow Setpoint, Store Value of 3 in variable Press_2
  {
    Press_2 = 3;
  }
  /******************************************************/
  
  Serial.println(Brightness_Blue);
  if (Brightness_Blue > Blue_Setpoint)                                                        //If Blue sensor shows brightness > Blue Setpoint, Store Value of 4 in variable Press_2
  {
    Press_2 = 4;
  }
 Serial.println(Press_2);
 
 /*******************************************************/
 delay(1000);                                                                                  //Delay to make the play time = 15 sec
  if (Press_1 == 1)                                                                            //If Press_1 = 1, First Press was red
  {
    for(pos_1 = 35; pos_1 >=1 ; pos_1 -= 1)                                                        //Press Red again
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
  
  if (Press_1 == 2)                                                                            //If Press_1 = 2, First Press Was Green
  {
    for(pos_1 = 35; pos_1 <= 70; pos_1 += 1)                                                    //Press Green again
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
  
  if (Press_1 == 3)                                                                                //If Press_1 = 3, first press was yellow
  {
    for(pos_2 = 35; pos_2 >= 1; pos_2 -= 1)                                                        //Press Yellow again
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
  if (Press_1 == 4)                                                                                //If Press_1 = 4, first press was blue
  {
    for(pos_2 = 35; pos_2 <= 70; pos_2 += 1)                                                          //Press Blue again
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
  delay(1000);                                                                                //Delay to make play time = 15 sec
    if (Press_2 == 1)                                                                          //If Press_2 = 1, Red is on
  {
    for(pos_1 = 35; pos_1 >=1 ; pos_1 -= 1)                                                    //Press Red
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
  
  if (Press_2 == 2)                                                                        //If Press_2 = 2, Green is on
  {
    for(pos_1 = 35; pos_1 <= 70; pos_1 += 1)                                                //Press Green
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
  
  if (Press_2 == 3)                                                                          //If Press_2 = 3, Yellow is on
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
  if (Press_2 == 4)                                                                      //If Press_2 = 4, Blue is on
  {
    for(pos_2 = 35; pos_2 <= 70; pos_2 += 1)                                              //Press Blue
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
/*********************************************************************************/

int Compare_Brightness_3(int pos_1, int pos_2)                                          //Determine which color is the 3rd illuminated
{
  Serial.println(Brightness_Red);  
  if (Brightness_Red > Red_Setpoint)                                                        //If Red sensor shows brightness > Red Setpoint, Store Value of 1 in variable Press_3
  {
    Press_3 = 1;
  }
  /*******************************************************/
  
  Serial.println(Brightness_Green);
  if (Brightness_Green > Green_Setpoint)                                                      //If Green sensor shows brightness > Green Setpoint, Store Value of 2 in variable Press_3
  {
    Press_3 = 2;
  }
  /*******************************************************/
  
 Serial.println(Brightness_Yellow);
  if (Brightness_Yellow >Yellow_Setpoint)                                                    //If Yellow sensor shows brightness > Yellow Setpoint, Store Value of 3 in variable Press_3
  {
    Press_3 = 3;
  }
  /******************************************************/
  
  Serial.println(Brightness_Blue);
  if (Brightness_Blue > Blue_Setpoint)                                                        //If Blue sensor shows brightness > Blue Setpoint, Store Value of 4 in variable Press_3
  {
    Press_3 = 4;
  }
 Serial.println(Press_3);
 
 /*******************************************************/
 delay(1000);                                                                              //Delay to make play time = 15 sec
  if (Press_1 == 1)                                                                          //If Press_1 = 1, first press was red
  {
    for(pos_1 = 35; pos_1 >=1 ; pos_1 -= 1)                                                //Press Red again
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
  
  if (Press_1 == 2)                                                                            //If Press_1 = 2, first press was green
  {
    for(pos_1 = 35; pos_1 <= 70; pos_1 += 1)                                              //Press green again
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
  
  if (Press_1 == 3)                                                                          //If Press_1 = 3, first press was yellow
  {
    for(pos_2 = 35; pos_2 >= 1; pos_2 -= 1)                                                      //Press yellow again
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
  if (Press_1 == 4)                                                                          //If Press_1 = 4, first press was blue
  {
    for(pos_2 = 35; pos_2 <= 70; pos_2 += 1)                                                //Press blue again
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
  delay(1000);                                                                              //delay to make play time = 15 sec
    if (Press_2 == 1)                                                                        //If Press_2 = 1, second press was red
  {
    for(pos_1 = 35; pos_1 >=1 ; pos_1 -= 1)                                                    //Press red again
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
  
  if (Press_2 == 2)                                                                  //If Press_2 = 2, second press was green
  {
    for(pos_1 = 35; pos_1 <= 70; pos_1 += 1)                                              //Press green again
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
  
  if (Press_2 == 3)                                                              //If Press_2 = 3, second press was yellow
  {
    for(pos_2 = 35; pos_2 >= 1; pos_2 -= 1)                                        //Press yellow again
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
  if (Press_2 == 4)                                                        //If Press_2 = 4, second press was blue
  {
    for(pos_2 = 35; pos_2 <= 70; pos_2 += 1)                                          //Press blue again
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
  delay(1000);                                                                      //delay to make play time = 15 sec
    if (Press_3 == 1)                                                          //If Press_3 = 1, third press is red
  {
    for(pos_1 = 35; pos_1 >=1 ; pos_1 -= 1)                                              //Press red
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
  
  if (Press_3 == 2)                                                      //If Press_3 = 2, third press is green
  {
    for(pos_1 = 35; pos_1 <= 70; pos_1 += 1)                                        //press green
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
  
  if (Press_3 == 3)                                                              //If Press_3 = 3, third press is yellow
  {
    for(pos_2 = 35; pos_2 >= 1; pos_2 -= 1)                                            //press yellow
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
  if (Press_3 == 4)                                                          //If Press_4 = 4, third press is blue
  {
    for(pos_2 = 35; pos_2 <= 70; pos_2 += 1)                                            //press blue
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

