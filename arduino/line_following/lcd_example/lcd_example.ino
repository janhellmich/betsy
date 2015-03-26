#include <LiquidCrystal.h>

LiquidCrystal lcd(43, 45, 47, 49, 51, 53);
  
void setup()
{
 
  lcd.begin(16, 2);
  
}


void loop()
{

  lcd.setCursor(0,1);
  lcd.clear();
  lcd.print("Hello World");
  
  delay(500);
}
