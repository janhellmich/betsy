//define Front Gripper Pins
#define FGRIPPER1 42
#define FGRIPPER2 41


#define STOP 0
#define OPEN 1
#define CLOSE 2



void setup() 
{
  pinMode(47, OUTPUT);
  pinMode(44, INPUT);
  pinMode(FGRIPPER1, OUTPUT);
  pinMode(FGRIPPER2, OUTPUT);
  
  Serial.begin(9600);
  
}

void loop() 
{  
  digitalWrite(47, LOW);
  Serial.println("Beginning Program");
  front_gripper(OPEN);
  delay(5000);
  front_gripper(CLOSE);
  delay(5000);
  digitalWrite(47, HIGH);
  Serial.println("Enabling Uno");
  digitalWrite(47, LOW);
  
  while  (digitalRead(44) != HIGH)
  {}       //Do Nothing
  front_gripper(OPEN);
  
  Serial.println("Uno Complete");
  delay(10000);
  front_gripper(STOP);
  
  while  (1)
  {}
  
}


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
