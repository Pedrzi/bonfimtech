#include "functions.h"

extern byte zero;
extern LiquidCrystal lcd;
extern int pointer;
extern int horarios[][2]; // Matriz de horários
extern char *local;
extern int selection;

void SaveArrayToEEPROM() 
{
  int addr = 0;
  for (int row = 0; row < pointer; row++) {
    for (int col = 0; col < 2; col++) {
      EEPROM.update(addr, horarios[row][col]);
      addr++;
    }
  }
}

void ReadArrayFromEEPROM() 
{
  int horarios[][2] = {};
  int addr = 0;
  for (int row = 0; row < pointer; row++) {
    for (int col = 0; col < 2; col++) {
      horarios[row][col] = EEPROM.read(addr);
      addr++;
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
    byte segundos = 0; // Valores de 0 a 59
    byte minutos = 12; // Valores de 0 a 59
    byte horas = 16; // Valores de 0 a 23
    byte diadasemana = 1; // Valores de 0 a 6 (0=Domingo, 1 = Segunda...)
    byte diadomes = 30; // Valores de 1 a 31
    byte mes = 10; // Valores de 1 a 12
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
    
    /* 
    //Mostra a data atual no display
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

    for(int i = 0; i < 9; i++)
    {
        Serial.println(horarios[i][0]);
        if(horarios[i][0] == segundos)
        {
            lcd.setCursor(11,0);
            lcd.print("O");
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
    delay(1000);

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
    if(local == "menu")
    {
         selection = 0;
    }
    else if (local == "turnos")
    {
        if (selection > 3 || selection == 0)
        {
            selection = 1;
        }
    }
    else if (local == "manha")
    {
        if (selection > 12 || selection < 4)
            selection = 4;
            
    }
    else if (local == "tarde")
    {
        if (selection > 18 || selection < 12)
            selection = 12;
    }
    else
    {
        selection = 0;
    }
}

