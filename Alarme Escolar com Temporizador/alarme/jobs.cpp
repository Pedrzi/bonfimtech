// func.h é incluido para o uso da classe LCD
#include "func.h"
#include "jobs.h"
#include <math.h>

// Váriável usada para criar um caractere especial de underline
byte barra[8] = {
    B11111,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
};

struct numArr;

extern int horarioSize;
extern int horarios[][2];

LiquidCrystal lcd(9, 8, 7, 6, 5, 4);
// converte a função millis do arduino para 64 bits
// sem essa conversão em alguns dias de uso o alarme sofrerá com overflow
uint64_t millis64()
{
    static uint32_t low32, high32;
    uint32_t new_low32 = millis();
    if (new_low32 < low32)
        high32++;
    low32 = new_low32;
    return (uint64_t)high32 << 32 | low32;
}

// Função criada para incializar os botões
void inicializarBotao()
{
    pinMode(BOTAOMOVER, INPUT_PULLUP);
    pinMode(BOTAOVALIDAR, INPUT_PULLUP);
    Serial.println("Buttons iniciado");
}

// Função criada para incializar o relay
void inicializarRelay()
{
    // pinMode(relay, OUTPUT);
    Serial.println("Relay iniciado");
}

// Função criada para inicializar a tela lcd
void inicializarLCD()
{
    analogWrite(3, contraste_LCD);
    lcd.createChar(0, barra);
    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    lcd.noCursor();
    lcd.noBlink();
    lcd.clear();
    Serial.println("LCD iniciado");
}

// Converte o número de decimal para BCD
byte ConverteParaBCD(byte val)
{
    return ((val / 10 * 16) + (val % 10));
}

// Converte de BCD para decimal
byte ConverteparaDecimal(byte val)
{
    return ((val / 16 * 10) + (val % 16));
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
        if (i == 2)
        {
            values[i] = ConverteparaDecimal(Wire.read() & 0b111111);
        }
        else
        {
            values[i] = ConverteparaDecimal(Wire.read());
        }
    }
}

// Função que da print no painel lcd o horário atual
void mostrarRelogio()
{
    int data[7];
    getRTC(data);
    // Imprime mensagem na primeira linha do display
    // Mostra a hora atual no display
    lcd.setCursor(4, 0);
    if (data[2] < 10)
    {
        lcd.print("0");
    }
    lcd.print(data[2]);
    lcd.print(":");
    if (data[1] < 10)
    {
        lcd.print("0");
    }
    lcd.print(data[1]);
    lcd.print(":");
    if (data[0] < 10)
    {
        lcd.print("0");
    }
    lcd.print(data[0]);
}

uint64_t moverTimer = 0;
uint64_t validarTimer = 0;
// Checa e retorna se algum botão foi apertado.
boolPair checarBotoes()
{
    bool mov = false;
    bool val = false;
    if (millis64() - moverTimer > BUTTONWAITTIME && !digitalRead(12))
    {
        moverTimer = millis64();
        mov = true;
    }

    if (millis64() - validarTimer > BUTTONWAITTIME && !digitalRead(11))
    {
        validarTimer = millis64();
        val = true;
    }

    return {val, mov};
}

void mudarHorario(byte minutos, byte horas)
{
    byte segundos = 30; // Valores de 0 a 59
    Wire.beginTransmission(DS1307_ADDRESS);
    // Stop no CI para que o mesmo possa receber os dados
    Wire.write(ZERO);

    // As linhas abaixo escrevem no CI os valores de
    // data e hora que foram colocados nas variaveis acima
    Wire.write(ConverteParaBCD(segundos));
    Wire.write(ConverteParaBCD(minutos));
    Wire.write(ConverteParaBCD(horas));
    Wire.write(ZERO); // Start no CI
    Wire.endTransmission();
    Serial.println("Data alterada");
}

// divide um número de duas casas decimais em dois números
// EX: 15 é dividido em 1 e 5
numArr dividirEmDois(int num)
{
    int primeiroDigito = floor(num / 10);
    int segundoDigito = num - primeiroDigito * 10;
    numArr digitos;
    digitos.num1 = primeiroDigito;
    digitos.num2 = segundoDigito;
    return digitos;
}

void SaveArrayToEEPROM()
{
    int addr = 1;
    for (int row = 0; row < horarioSize; row++)
    {
        for (int col = 0; col < 2; col++)
        {
            EEPROM.update(addr, horarios[row][col]);
            addr++;
        }
    }
}

void ReadArrayFromEEPROM()
{
    int addr = 1;
    for (int row = 0; row < horarioSize; row++)
    {
        for (int col = 0; col < 2; col++)
        {
            horarios[row][col] = EEPROM.read(addr);
            addr++;
            Serial.println(horarios[row][col]);
        }
    }
}