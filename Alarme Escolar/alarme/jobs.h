#ifndef JOBS_H
#define JOBS_H
#include <Arduino.h>
#include <LiquidCrystal.h>  

#define BUTTONWAITTIME 200
#define LIMITETOQUE 30
// pins usados

#define RELAY 10
#define BOTAOVALIDAR 11
#define BOTAOMOVER 12
#define LCD_RS 9
#define LCD_ENABLE 8
#define LCD_D4 7
#define LCD_D5 6
#define LCD_D6 5
#define LCD_D7 4
#define LCD_VEE 3

/*
struct criado para retornar os valores dos botões com uma só chamada de função.
```cpp
return {validar, mover}
```
*/ 
struct boolPair
{
    bool validar;
    bool mover;
};


struct numArr {
  int num1;
  int num2;
};

boolPair checarBotoes();


void inicializarBotao();
void inicializarRelay();
void inicializarLCD();
void mudarHorario(byte minutos, byte horas, byte dia);
void mostrarRelogio();
void SaveArrayToEEPROM();
void ReadArrayFromEEPROM();

byte ConverteparaDecimal(byte val);
byte ConverteParaBCD(byte val);

/* divide um número em sua dezena e uniadade
eg. dividirEmDois(25) = numArr {2, 5}
*/ 
numArr dividirEmDois(int num);

int getRTC(int values[]);

uint64_t millis64();

#endif 
