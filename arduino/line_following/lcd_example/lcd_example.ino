#include <LiquidCrystal.h>

// blue on the top left, purple top right

LiquidCrystal lcd(49, 51, 53, 52, 50, 48);
  
void setup()
{
 
  lcd.begin(16, 2);
  
}


void loop()
{

  lcd.setCursor(0,1);
  lcd.clear();
  lcd.print("IEEE");
  
  delay(500);
}
