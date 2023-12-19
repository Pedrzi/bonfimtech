#include "functions.h"

char layers[][16] = {"menu", "turnos", "manha", "tarde", "edição", "configuração"};
char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};

LiquidCrystal lcd(9, 8, 7, 6, 5, 4);

const byte zero = 0x00;;

int alarmeDuracao = 4;
int alarmeDuracaoTemporario = alarmeDuracao;
int horarios[][2] = {
    { 7,  0}, { 7, 50},
    { 8, 40}, { 9, 30},
    { 9, 50}, {10, 40},
    {11, 30}, {12, 10},
    {13,  0}, {13, 40},
    {14, 20}, {15,  0},
    {15, 20}, {16,  0}
};
int horarioSize = 14;

bool tocarAlarme;

byte barra[8] = {
    B11111,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
};

extern int horarios[][2]; // Matriz de horários
extern char *local;
extern int selection;
extern bool alarm;
extern unsigned long alarme;
extern bool alarmando;

void mudarHorario()
{
    byte segundos = 20; // Valores de 0 a 59
    byte minutos = 57; // Valores de 0 a 59
    byte horas = 14; // Valores de 0 a 23
    byte diadasemana = 2; // Valores de 0 a 6 (0=Domingo, 1 = Segunda...)
    byte diadomes = 12; // Valores de 1 a 31
    byte mes = 12; // Valores de 1 a 12
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
    Serial.println("Data alterada");
}

void Mostrarelogio()
{
    int data[7];
    getRTC(data);

    // Imprime mensagem na primeira linha do display
    // Mostra a hora atual no display
    lcd.setCursor(4, 0);
    if (data[2] < 10)
    {lcd.print("0");}
    lcd.print(data[2]);
    lcd.print(":");
    if (data[1] < 10)
    {lcd.print("0");}
    lcd.print(data[1]);
    lcd.print(":");
    if (data[0] < 10)
    {lcd.print("0");}
    lcd.print(data[0]);

}

void Checarhora()
{
    int horas[7];
    getRTC(horas);

    if(horas[3] < 6 && horas[3] > 0 && !tocarAlarme)
    {
        for(int i = 0; i < 15; i++)
        {
            if(horarios[i][0] == horas[2])
            {
                if(horarios[i][1] == horas[1])
                {
                    tocarAlarme = true;
                }
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
    Serial.println("Buttons started");
}

void StartRelay()
{
    pinMode(relay, OUTPUT);
    Serial.println("Relay started");
}

bool StartLCD()
{
    bool reset = false;
    analogWrite(3, Contrast);
    lcd.createChar(0, barra);
    lcd.begin(16, 2);
    lcd.setCursor(0,0);
    lcd.noCursor();
    lcd.noBlink();

    LoadingAnim();
    if(!digitalRead(mudar) && !digitalRead(confirmar)){
        reset = true;
    }
    lcd.clear();
    Serial.println("LCD started");
    return reset;
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
        lcd.print("Iniciando.");
        break;
        case 2:
        lcd.print("Iniciando..");
        break;
        case 3:
        lcd.print("Iniciando...");
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
         if(selection < 30 || selection > 30)
         {
            selection = 0;
         }
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
    else if(local == layers[5])
    {
        if (selection > 33 || selection < 31)
            selection = 31;
    }
}

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

int getRTC(int values[])
{
    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(zero);
    Wire.endTransmission();
    Wire.requestFrom(DS1307_ADDRESS, 7);

    for (int i = 0; i < 7; i++)
    {
        if(i == 2)
        {
            values[i] = ConverteparaDecimal(Wire.read() & 0b111111); 
        }
        else
        {
            values[i] = ConverteparaDecimal(Wire.read());
        }
    }
    
}