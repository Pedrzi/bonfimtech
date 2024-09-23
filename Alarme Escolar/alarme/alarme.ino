/*
    Para o próximo DEV a usar esse código.

    Eu não sei oq estava pensando enquanto fazia essa coisa mantida
    apenas por magia negra e sofrimento humano. Boa sorte tentando entender.
*/
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
    inicializarRelay();
    //SaveArrayToEEPROM();
    ReadArrayFromEEPROM();
    tempoToque = EEPROM.read(90);
}

void loop()
{
    handleLayers();
    Checarhora();
    handleAlarm();
    
}

bool tocado = false;
void Checarhora()
{
    int horas[7];
    getRTC(horas);

    for (int x = 0; x < 15; x++)
    {
    	if(horas[2] == horarios[x][0] && horas[1] == horarios[x][1])
        {
        	Serial.println("2");
        	if(!tocado)
        	{
          		Serial.println("1");
          		tocarAlarme = true;
        	}
      	}
    }
}

unsigned long alarmeTimer = 0;
void handleAlarm()
{
	if (tocarAlarme)
  	{
    	if (!tocado)
    	{
        	digitalWrite(10, HIGH);
        	alarmeTimer = millis64();
		}
		if (millis64() - alarmeTimer > tempoToque*1000)
			tocarAlarme = false;
		tocado = true;
		if (millis64() - alarmeTimer > 120000)
			tocado = false;
		
	}
	else digitalWrite(10, LOW);
	}
