#include "func.h"


const int8_t relay          = 10;
const int8_t botaoValidar   = 11;
const int8_t botaoMover     = 12;

extern LiquidCrystal lcd;

// Váriável usada para criar um caractere especial de underline
byte barra[8] = {B11111,B00000,B00000,B00000,B00000,B00000,B00000,};

// converte a função millis do arduino para 64 bits
// sem essa conversão em alguns dias de uso o alarme sofrerá com overflow
uint64_t millis64()
{
    static uint32_t low32, high32;
    uint32_t new_low32 = millis();
    if (new_low32 < low32) high32++;
    low32 = new_low32;
    return (uint64_t) high32 << 32 | low32;
}


// Função criada para incializar os botões
void inicializarBotao()
{
    pinMode(botaoMover,   INPUT_PULLUP);
    pinMode(botaoValidar, INPUT_PULLUP);
    Serial.println("Buttons iniciado");
}

// Função criada para incializar o relay
void inicializarRelay()
{
    pinMode(relay, OUTPUT);
    Serial.println("Relay iniciado");
}


// Função criada para inicializar a tela lcd
void inicializarLCD()
{
    analogWrite(3, contraste_LCD);
    lcd.createChar(0, barra);
    lcd.begin(16, 2);
    lcd.setCursor(0,0);
    lcd.noCursor();
    lcd.noBlink();
    lcd.clear();
    Serial.println("LCD iniciado");
}


// Converte o número de decimal para BCD
byte ConverteParaBCD(byte val)
{ 
 return ( (val/10*16) + (val%10) );
}


// Converte de BCD para decimal
byte ConverteparaDecimal(byte val)
{ 
 return ( (val/16*10) + (val%16) );
}


// Função que retorna o horário salvo no rtc
// Usada para mostrar o relógio no menu
int getRTC(int values[])
{
    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(ZERO);
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