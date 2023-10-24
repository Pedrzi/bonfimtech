// C++ code
//
  #define ledpt 13
  #define ledpl 10
  #define pl 11 
  #define pt 12

  
void setup()
{
  Serial.begin (9600);
  pinMode(ledpt, OUTPUT);
  pinMode(ledpl, OUTPUT);
  pinMode(pt, INPUT_PULLUP);
  pinMode(pl, INPUT_PULLUP);
}

void loop()
{

 Serial.print (digitalRead(pt));
  
  if(digitalRead(pt) == LOW  && digitalRead(pl) == HIGH) {
   
      digitalWrite(ledpt, HIGH);
      delay(3500);
      digitalWrite(ledpt, LOW);
      
      
    }

  if(digitalRead(pl) == LOW  && digitalRead(pt) == HIGH) {
   
      digitalWrite(ledpl, HIGH);
      delay(3500);
      digitalWrite(ledpl, LOW);
     
      
    } 

  
  

}

 
