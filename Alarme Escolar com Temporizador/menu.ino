#define mudar     2
#define confirmar 4

int selecao = 0;
int local = 0;
int hora[] = {};

void setup() {
  Serial.begin(9600);
  pinMode(mudar, INPUT_PULLUP);
  pinMode(confirmar, INPUT_PULLUP);

}

void loop() {
  if(local == 0){

     if(digitalRead(mudar) == LOW){
        if(selecao < 1){
          selecao++;
        }else{
          selecao = 0;
        }
    }

     if(digitalRead(confirmar) == LOW){
        local = selecao+1;
        selecao = 0;
     }

  if(local == 1){
    int hora = 0; // 1
    int minuto = 0; // 0

    Serial.println();

    while(local == 1){
      delay(200);
      Serial.print(selecao);
      Serial.println();
      Serial.print(hora);
      Serial.print(":");
      Serial.print(minuto); 
      Serial.println();
      Serial.println();
      if(digitalRead(mudar) == LOW){
        if(selecao < 3){
          selecao++;
        }else{
          selecao = 0;
        }
      }

      switch(selecao){
        case 0:
          if(digitalRead(confirmar) == LOW){
            
            if(minuto < 59){
              minuto++;
            }else{
              minuto = 0;
            }
          }
          break;
        case 1:
          if(digitalRead(confirmar) == LOW){
            
            if(hora < 23){
              hora++;
            }else{
              hora = 0;
            }
          }
          break;
        case 2: //save function write later

          break;
        case 3: // quit
          if(digitalRead(confirmar) == LOW){
            local = 0;
            selecao = 0;
          }
          break;
        }
      }
    }
  }

  Serial.print(selecao);
  Serial.print(" ");
  Serial.print(local);
  Serial.println();

  delay(200);
}

