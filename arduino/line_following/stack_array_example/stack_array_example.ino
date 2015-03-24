// This example shows how to use a StackArray. make sure you have installed the library

#include <StackArray.h>

StackArray<int> stack;

void setup()
{
  delay(500);
  Serial.begin(9600);
  delay(1000);
}

void loop()
{
  
  for (int i = 0; i < 5; i++) {
    stack.push(i);
  }
  while (stack.isEmpty() == false) {
    Serial.println(stack.pop());
  }
  Serial.println("stack is empty");
  
  delay(2000);
}
