#include <EEPROM.h>
#include "Wire.h"
#include <LiquidCrystal.h> 
#include "RTClib.h"

LiquidCrystal lcd(9,8,7,6,5,4);

#define DS1307_ADDRESS 0x68
#define mudar     A3
#define confirmar A2
#define numRows  64
#define numCols   2 

int selecao = 0;
int local = 0;
int pointer = 0;
int horarios[64][2] = {};
int Contrast=130;
char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};
byte zero = 0x00;


RTC_DS1307 rtc;

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
  Wire.begin();
  analogWrite(3, Contrast);
  lcd.begin(16, 2);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  // SelecionaDataeHora();
  //EEPROM.write(98, 0);  // mudar indice do ponteiro
  pinMode(mudar, INPUT_PULLUP);
  pinMode(confirmar, INPUT_PULLUP);

  pointer = EEPROM.read(98);
  if(pointer > 0){
    readArrayFromEEPROM();
  }

  Serial.println("Ligando!");
 
  delay(100);
}

void loop() {
  DateTime now = rtc.now();
  
  if(local == 0){
    
     if(digitalRead(mudar) == LOW){
        if(selecao < 2){
          selecao++;
        }else{
          selecao = 0;
        }
    }

     if(digitalRead(confirmar) == LOW){
        local = selecao+1;
        selecao = 0;
     }
    lcd.setCursor(0, 1);
    
    switch (selecao){
      case 0:
        lcd.clear();
        Mostrarelogio();
        lcd.setCursor(3, 1);
        lcd.print("ADICIONAR.");
        
        break;
      case 1:
        lcd.clear();
        Mostrarelogio();
        lcd.setCursor(3, 1);
        lcd.print("VISUALIZAR");

        break;
      case 2:
        lcd.clear();
        Mostrarelogio();
        lcd.setCursor(4, 1);
        lcd.print("REMOVER.");

        break;
    }


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
            Serial.println("Salvo!");
            delay(100);
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
    Serial.print(now.second());
    Serial.print(selecao);
    Serial.print(" ");
    Serial.print(local);
    Serial.print(" ");
    Serial.print("horario: ");
    Serial.print(horarios[0][0]);
    Serial.print(":");
    Serial.print(horarios[0][1]);
    Serial.println();
    
    delay(200);
}


void Mostrarelogio()
{
 Wire.beginTransmission(DS1307_ADDRESS);
 Wire.write(zero);
 Wire.endTransmission();
 Wire.requestFrom(DS1307_ADDRESS, 7);
 int segundos = ConverteparaDecimal(Wire.read());
 int minutos = ConverteparaDecimal(Wire.read());
 int horas = ConverteparaDecimal(Wire.read() & 0b111111); 
 int diadasemana = ConverteparaDecimal(Wire.read()); 
 int diadomes = ConverteparaDecimal(Wire.read());
 int mes = ConverteparaDecimal(Wire.read());
 int ano = ConverteparaDecimal(Wire.read());
 // Imprime mensagem na primeira linha do display
 // Mostra a hora atual no display
 lcd.setCursor(4, 0);
 if (horas < 10)
 {lcd.print("0");}
 lcd.print(horas);
 lcd.print(":");
 if (minutos < 10)
 {lcd.print("0");}
 lcd.print(minutos);
 lcd.print(":");
 if (segundos < 10)
 {lcd.print("0");}
 lcd.print(segundos);
 /* 
 // Mostra a data atual no display
 lcd.setCursor(0, 3);
 lcd.print("Data : ");
 lcd.setCursor(7,3);
 if (diadomes < 10)
 {lcd.print("0");}
 lcd.print(diadomes);
 lcd.print(".");
 if (mes < 10)
 {lcd.print("0");}
 lcd.print(mes);
 lcd.print(".");
 lcd.print(ano);
  
 //Mostra o dia da semana no display
 lcd.setCursor(17, 3);
 switch(diadasemana)
 {
 case 0:lcd.print("Dom");
 break;
 case 1:lcd.print("Seg");
 break;
 case 2:lcd.print("Ter");
 break;
 case 3:lcd.print("Qua");
 break;
 case 4:lcd.print("Qui");
 break;
 case 5:lcd.print("Sex");
 break;
 case 6:lcd.print("Sab");
 }
 */
}

void SelecionaDataeHora() // Seta a data e a hora do DS1307
{
 byte segundos = 10; // Valores de 0 a 59
 byte minutos = 32; // Valores de 0 a 59
 byte horas = 9; // Valores de 0 a 23
 byte diadasemana = 2; // Valores de 0 a 6 (0=Domingo, 1 = Segunda...)
 byte diadomes = 24; // Valores de 1 a 31
 byte mes = 10; // Valores de 1 a 12
 byte ano = 23; // Valores de 0 a 99
 Wire.beginTransmission(DS1307_ADDRESS);
 // Stop no CI para que o mesmo possa receber os dados
 Wire.write(zero); 
 
 // As linhas abaixo escrevem no CI os valores de 
 // data e hora que foram colocados nas variaveis acima
 Wire.write(ConverteParaBCD(segundos));
 Wire.write(ConverteParaBCD(minutos));
}

byte ConverteParaBCD(byte val)
{ 
 // Converte o número de decimal para BCD
 return ( (val/10*16) + (val%10) );
}
 
byte ConverteparaDecimal(byte val) 
{ 
 // Converte de BCD para decimal
 return ( (val/16*10) + (val%16) );
}
