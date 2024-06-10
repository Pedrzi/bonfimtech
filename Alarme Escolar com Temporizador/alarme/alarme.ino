#include "func.h"
#include "jobs.h"

bool tocarAlarme = false;
int tempoToque = 1;
extern int horarios[][2];
void setup()
{
    Serial.begin(9600);
    inicializarBotao();
    inicializarLCD();
    ReadArrayFromEEPROM();
    tempoToque = EEPROM.read(ENDERECODETOQUE);
}

void loop()
{
    handleLayers();
    Checarhora();
    handleAlarm();
}

void Checarhora()
{
    int horas[7];
    getRTC(horas);

    if (horas[3] < 6 && horas[3] > 0 && !tocarAlarme)
    {
        for (int i = 0; i < 15; i++)
        {
            if (horarios[i][0] == horas[2])
            {
                if (horarios[i][1] == horas[1])
                {
                    tocarAlarme = true;
                }
            }
        }
    }
}
bool tocado = false;
uint64_t alarmeTimer = 0;
void handleAlarm()
{
    if (!tocarAlarme)
    {
        digitalWrite(RELAY, HIGH);
        tocado = false;
    }

    if (tocarAlarme && !tocado)
    {
        digitalWrite(RELAY, LOW);
    }

    if (tocarAlarme)
    {
        if (millis64() - alarmeTimer < TEMPOTOQUE*1000)
            return;
        alarmeTimer = millis64();
        tocado = true;
    }
}