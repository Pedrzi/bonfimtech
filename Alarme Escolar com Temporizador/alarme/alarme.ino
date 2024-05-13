#include "func.h"

extern LiquidCrystal lcd;

extern const uint8_t relay;
extern const uint8_t botaoValidar;
extern const uint8_t botaoMover;
struct boolPair;

void setup()
{
    Serial.begin(9600);
    inicializarBotao();
    inicializarLCD();
    
}

void loop()
{   
    handleLayers();
    render();
}

