#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Arduino.h>
#include <LiquidCrystal.h>  
#include <EEPROM.h>
#include "Wire.h"

#define DS1307_ADDRESS 0x68
#define mudar     A3 // Botão de mudar
#define confirmar A2 // Botão de confirmar
#define numRows   64 // Número de linhas da matriz de horários
#define numCols    2 // número de colunas da matriz de horáriios
#define Contrast 130 // contraste da tela LCD
#define pointerIndex 98 // Índice do pontador




void SaveArrayToEEPROM();
void ReadArrayFromEEPROM();
byte ConverteParaBCD(byte val);
byte ConverteparaDecimal(byte val);
void SelecionaDataeHora();
void Mostrarelogio();
void Checarhora();
uint64_t millis64();
void StartLCD();
void LoadingAnim();
void StartEEPROM();
void StartButtons();

#endif