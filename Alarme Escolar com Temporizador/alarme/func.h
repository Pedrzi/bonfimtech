#ifndef FUNC_H
#define FUNC_H

#include <Arduino.h>
#include <EEPROM.h>
#include <Wire.h>

// Endereço ocupado pelo RTC no i2c
#define DS1307_ADDRESS 0x68
// Contraste da tela lcd
// Quanto maior o valor menor será o contraste
// Padrão = 90
#define contraste_LCD 90
#define ZERO 0x00
#define BUTTONWAITTIME 200
#define ENDERECODETOQUE 90
#define TEMPOTOQUE EEPROM.read(ENDERECODETOQUE)

void handleLayers();
void renderMenu(char layerTitle[17]);
void editor(bool valid, bool move, int current);
void renderEditor();
void mostrarHorarios(bool vali, bool mov, int current);
void mudarTempoDeToque(bool v, bool m);
void renderHorarios();
void renderMudarDuracao();


#endif