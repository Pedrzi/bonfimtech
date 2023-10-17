#include <EEPROM.h>

#define mudar     2
#define confirmar 4

int selecao = 0;
int local = 0;

int numRows = 64;
int numCols = 2;
int horarios[64][2] = {};
int pointer = 0;

/*
int concatenarInt(int num1, int num2){
  String strNum1 = String(num1);
  String strNum2 = String(num2);

  String concatenated = strNum1 + strNum2;

  int result = concatenated.toInt();

  return result;
}
*/

void saveArrayToEEPROM() {
  int addr = 0;
  for (int row = 0; row < pointer; row++) {
    for (int col = 0; col < numCols; col++) {
      EEPROM.update(addr, horarios[row][col]);
      addr++;
    }
  }
}

void readArrayFromEEPROM() {
  int addr = 0;
  for (int row = 0; row < pointer; row++) {
    for (int col = 0; col < numCols; col++) {
      horarios[row][col] = EEPROM.read(addr);
      addr++;
    }
  }
}


void setup() {
  Serial.begin(9600);
  pinMode(mudar, INPUT_PULLUP);
  pinMode(confirmar, INPUT_PULLUP);
  
  pointer = EEPROM.read(98);
  if(pointer > 0){
    readArrayFromEEPROM();
  }
  

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
        case 0: // mudar minutos
          if(digitalRead(confirmar) == LOW){
            
            if(minuto < 59){
              minuto++;
            }else{
              minuto = 0;
            }
          }
          break;
        case 1: // Mudar horas
          if(digitalRead(confirmar) == LOW){
            
            if(hora < 23){
              hora++;
            }else{
              hora = 0;
            }
          }
          break;
        case 2: //save function write later
          if(digitalRead(confirmar) == LOW){
            horarios[pointer][0] = hora;
            horarios[pointer][1] = minuto;
            pointer++;

            EEPROM.write(98, pointer);

            saveArrayToEEPROM();
          }
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
