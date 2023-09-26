#define confirma 2
#define direita  5
#define cima     8

int segundo_um      = 0; // 0
int segundo_dois    = 0; // 1
int minuto_um       = 0; // 2
int minuto_dois     = 0; // 3
int hora_um         = 0; // 4
int hora_dois       = 0; // 5

int x = 0;
int y = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(confirma, INPUT_PULLUP);
  pinMode(direita , INPUT_PULLUP);
  pinMode(cima    , INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {

  if(x >= 6){
    x = 0;
  }

  if(digitalRead(direita) == LOW){
    x++;
    y = 0;
    delay(500);
  }

  switch(x){
    case 0:
      if(y > 9){
        y = 0;
      }

      if(digitalRead(cima) == LOW){
        y++;
        delay(500);
      }

      if(digitalRead(confirma) == LOW){
        segundo_um = y;
      }

      break;
    case 1:
      if(y > 6){
        y = 0;
      }

      if(digitalRead(cima) == LOW){
        y++;
        delay(500);
      }

      if(digitalRead(confirma) == LOW){
        segundo_dois = y;
      }

      break;
    case 2:
      if(y > 9){
        y = 0;
      }

      if(digitalRead(cima) == LOW){
        y++;
        delay(500);
      }

      if(digitalRead(confirma) == LOW){
        minuto_um = y;
      }

      break;
    case 3:
      if(y > 6){
          y = 0;
        }

        if(digitalRead(cima) == LOW){
        y++;
        delay(500);
      }

      if(digitalRead(confirma) == LOW){
        minuto_dois = y;
      }

      break;
    
    case 4:
      if(y > 9){
          y = 0;
        }

        if(digitalRead(cima) == LOW){
        y++;
        delay(500);
      }

      if(digitalRead(confirma) == LOW){
        hora_um = y;
      }

      break;
    
    case 5:

      if(y > 6){
          y = 0;
        }

        if(digitalRead(cima) == LOW){
        y++;
        delay(500);
      }

      if(digitalRead(confirma) == LOW){
        hora_dois = y;
      }

      break;
  }
  Serial.print(x);
  Serial.print(" ");
  Serial.print(y);
  Serial.print(" ");
  Serial.print(hora_dois);
  Serial.print(hora_um);
  Serial.print(":");
  Serial.print(minuto_dois);
  Serial.print(minuto_um);
  Serial.println();

}
