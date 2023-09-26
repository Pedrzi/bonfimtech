#include <LiquidCrystal.h> 
int Contrast=130;
 LiquidCrystal lcd(9,8,7,6,5,4);  

 void setup()
 {
    analogWrite(3,Contrast);
     lcd.begin(16, 2);
 } 
     void loop()
 {
   
     lcd.setCursor(0, 0);
     lcd.print("HELLO WORLD!");
   
    lcd.setCursor(0, 1);
     lcd.print("SUBSCRIBE :]");

   /*lcd.scrollDisplayLeft();
   delay(400); */
   

   
}
