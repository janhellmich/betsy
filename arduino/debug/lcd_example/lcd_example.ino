#include <LiquidCrystal.h>

// blue on the top left, purple top right

LiquidCrystal lcd(49, 51, 53, 52, 50, 48);
  
void setup()
{
 
  lcd.begin(16, 2);
  
}


void loop()
{

  lcd.clear();
  lcd.setCursor(10,1);
  
  lcd.print("IEEE");
  
  delay(500);
}
