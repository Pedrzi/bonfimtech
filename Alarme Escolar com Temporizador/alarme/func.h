#ifndef FUNC_H
#define FUNC_H


#include <Arduino.h>
#include <LiquidCrystal.h>  
#include <EEPROM.h>
#include "Wire.h"

// Endereço ocupado pelo RTC no i2c
#define DS1307_ADDRESS 0x68
// Contraste da tela lcd
// Quanto maior o valor menor será o contraste
// Padrão = 90
#define contraste_LCD 90
#define ZERO 0x00
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


uint64_t millis64();
void inicializarBotao();
void inicializarRelay();
void inicializarLCD();
boolPair checarBotoes();
void handleLayers();
void render();
byte ConverteparaDecimal(byte val);
byte ConverteParaBCD(byte val);
void Mostrarelogio();
int getRTC(int values[]);
void specialLayers(int layer);

#endif