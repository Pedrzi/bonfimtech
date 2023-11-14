#include "functions.h"

extern byte zero;
extern LiquidCrystal lcd;
extern int pointer;
extern int horarios[][2]; // Matriz de horários
extern char *local;
extern int selection;
extern char layers[4][16];
extern int horarioSize;

void SaveArrayToEEPROM() 
{
  int addr = 0;
  for (int row = 0; row < horarioSize; row++) {
    for (int col = 0; col < 2; col++) {
      EEPROM.update(addr, horarios[row][col]);
      addr++;
    }
  }
}

void ReadArrayFromEEPROM() 
{
  int addr = 0;
  for (int row = 0; row < horarioSize; row++) {
    for (int col = 0; col < 2; col++) {
      horarios[row][col] = EEPROM.read(addr);
      addr++;
      Serial.println(horarios[row][col]);
    }
  }
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

void SelecionaDataeHora()
{
    byte segundos = 25; // Valores de 0 a 59
    byte minutos = 57; // Valores de 0 a 59
    byte horas = 0; // Valores de 0 a 23
    byte diadasemana = 2; // Valores de 0 a 6 (0=Domingo, 1 = Segunda...)
    byte diadomes = 14; // Valores de 1 a 31
    byte mes = 11; // Valores de 1 a 12
    byte ano = 23; // Valores de 0 a 99
    Wire.beginTransmission(DS1307_ADDRESS);
    // Stop no CI para que o mesmo possa receber os dados
    Wire.write(zero); 
    
    // As linhas abaixo escrevem no CI os valores de 
    // data e hora que foram colocados nas variaveis acima
    Wire.write(ConverteParaBCD(segundos));
    Wire.write(ConverteParaBCD(minutos));
    Wire.write(ConverteParaBCD(horas));
    Wire.write(ConverteParaBCD(diadasemana));
    Wire.write(ConverteParaBCD(diadomes));
    Wire.write(ConverteParaBCD(mes));
    Wire.write(ConverteParaBCD(ano));
    Wire.write(zero); //Start no CI
    Wire.endTransmission(); 
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

}

void Checarhora()
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

    for(int i = 0; i < 15; i++)
    {
        //Serial.println(horarios[i][0]);
        if(horarios[i][0] == horas)
        {
            if(horarios[i][1] == minutos)
            {
                Serial.println("ALARMEEEEE!!!!!!!!!!");
            }
        }
    }
    
}

uint64_t millis64() {
    static uint32_t low32, high32;
    uint32_t new_low32 = millis();
    if (new_low32 < low32) high32++;
    low32 = new_low32;
    return (uint64_t) high32 << 32 | low32;
}

void StartButtons()
{
    pinMode(mudar, INPUT_PULLUP);
    pinMode(confirmar, INPUT_PULLUP);
}

void StartEEPROM()
{
    if (EEPROM.read(pointerIndex) > 64)
    {
        EEPROM.write(pointerIndex, 0);
    }

    pointer = EEPROM.read(pointerIndex);

    if(pointer > 0)
    {
        ReadArrayFromEEPROM();
    }

    Serial.print("Pointer index is at: ");
    Serial.println(pointer);
}

void StartLCD()
{
    analogWrite(3, Contrast);

    lcd.begin(16, 2);

    lcd.setCursor(0,0);
    lcd.print("Ola!");
    //delay(1000);

    //LoadingAnim();
    lcd.clear();
}

void LoadingAnim()
{
    int dots = 1;
    for(int i = 0; i < 7; i++){
        lcd.clear();
        if(dots > 3){
        dots = 1;
        }
        switch (dots)
        {
        case 1:
        lcd.print("Loading.");
        break;
        case 2:
        lcd.print("Loading..");
        break;
        case 3:
        lcd.print("Loading...");
        break;
        }
        dots++;
        delay(500);
        
    }
}

void selecChanger()
{   
    if(local == layers[0])
    {
         selection = 0;
    }
    else if (local == layers[1])
    {
        if (selection > 3 || selection == 0)
        {
            selection = 1;
        }
    }
    else if (local == layers[2])
    {
        if (selection > 12 || selection < 4)
            selection = 4;     
    }
    else if (local == layers[3])
    {
        if (selection > 19 || selection < 13)
            selection = 13;
    }
    else if (local == layers[4])
    {
         if (selection > 23 || selection < 20)
            selection = 20;
    }
}

