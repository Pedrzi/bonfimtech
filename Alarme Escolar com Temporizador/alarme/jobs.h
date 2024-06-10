#ifndef JOBS_H
#define JOBS_H
#include <Arduino.h>
#include <LiquidCrystal.h>  

#define RELAY 10
#define BOTAOVALIDAR 11
#define BOTAOMOVER 12
#define BUTTONWAITTIME 200

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
void mudarHorario(byte minutos, byte horas);
void mostrarRelogio();
void SaveArrayToEEPROM();
void ReadArrayFromEEPROM();

byte ConverteparaDecimal(byte val);
byte ConverteParaBCD(byte val);

numArr dividirEmDois(int num);

int getRTC(int values[]);

uint64_t millis64();

#endif 
