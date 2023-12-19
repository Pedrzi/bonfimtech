#include "functions.h"
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "Wire.h"

extern int alarmeDuracao;
extern int alarmeDuracaoTemporario;
extern int horarios[][2];
extern char layers[][16];
extern LiquidCrystal lcd;
extern bool tocarAlarme;

bool alarmandoManual;
bool alarmeTocado;
bool alarmeTocando;
bool switch_alarme;
bool muda;
bool confirma;

char *local = layers[0];

int hora;
int minuto;
int selection;

unsigned long previousMillis;
unsigned long previousTime;

void setup()
{
    Serial.begin(9600);
    Wire.begin();

    //mudarHorario();
    StartButtons();
    StartRelay();
    
    if (!StartLCD())
    {
        ReadArrayFromEEPROM();
        alarmeDuracao = EEPROM.read(durationPointer);
    }
    else
    {
        SaveArrayToEEPROM();
        EEPROM.write(durationPointer, alarmeDuracao);
    }

    delay(100);
}

void loop()
{
    unsigned long currentTime = millis64();
    muda = false;
    confirma = false;

    if(!digitalRead(mudar) && currentTime - previousMillis > 200)
    {
        previousMillis = currentTime;
        muda = true;
        selection+=1;
        lcd.clear();
    }

    if(!digitalRead(confirmar) && currentTime - previousMillis > 200)
    {
        previousMillis = currentTime;
        confirma = true;
        lcd.clear();
    }

    handleAlarm(currentTime);
    handleInputs(currentTime);
    renderUI();
}

void handleAlarm(unsigned long currentTime) 
{
    // Handle alarm logic
    // Check alarm conditions, set/reset alarm, etc.

    if(muda && confirma && !tocarAlarme)
    {
        digitalWrite(relay, HIGH);
        switch_alarme = true;
        alarmandoManual = true;
    }
    else if(!tocarAlarme)
    {
        digitalWrite(relay, LOW);
        switch_alarme = false;
        alarmandoManual = false;
    }
    else
    {
        switch_alarme = false;
        alarmandoManual = false;
    }

    if(tocarAlarme && !alarmeTocado)
    {
        digitalWrite(relay, HIGH);
        alarmeTocado = true;
        alarmeTocando = true;
        previousTime = currentTime;
    }

    if(currentTime - previousTime > alarmeDuracao && alarmeTocando)
    {
        digitalWrite(relay, LOW);
        alarmeTocando = false;
    }

    if(currentTime - previousTime > 60000)
    {
        tocarAlarme = false;
        alarmeTocado = false;
    }

}

void handleInputs(unsigned long currentTime) 
{
    int index;

    if (local == layers[0])
    {   
        //pedaço de código que faz possível ter uma tela de configuração para a duração do alarme
        if (muda && !switch_alarme)
        {
            selection += 30;
        }
    }
    else if (local == layers[1])
    {
        switch (selection)
        {
        case 3:
            if (confirma && !switch_alarme)
            {
                local = layers[0];
            }
            break;
        }
    }    
    else if (local == layers[2]) // manha
    {
        if (selection < 12)
        {
            if (confirma && !switch_alarme)
            {
                // enviar horário para o editor de horários
                index = selection - 4;
                hora = horarios[index][0];
                minuto = horarios[index][1];
                local = layers[4];
            }
        }
        else
        {
            if (confirma && !switch_alarme)
            {
                local = layers[0];
            }
        }
    }
    else if (local == layers[3]) // tarde
    {
        if (selection < 19)
        { // 16 24 21
            if (confirma && !switch_alarme)
            {
                // enviar horário para o editor de horário
                index = selection - 5;
                hora = horarios[index][0];
                minuto = horarios[index][1];
                local = layers[4];
            }
            
           
        }
        else
            {
                if (confirma && !switch_alarme)
                {
                    local = layers[0];
                }
            }
    }
    else if (local == layers[4]) // editor
    {
        switch (selection)
        {
        case 20:
            if (confirma && !switch_alarme)
            {
                hora++;
                if (hora > 23)
                {
                    hora = 0;
                }
                Serial.println(hora);
                delay(200);
            }
            break;
        case 21:
            if (confirma && !switch_alarme)
            {

                minuto++;
                if (minuto > 59)
                {
                    minuto = 0;
                }
                Serial.println(minuto);
                delay(200);
            }
            break;
        case 22:
            if (confirma && !switch_alarme)
            {
                horarios[index][0] = hora;
                horarios[index][1] = minuto;
                SaveArrayToEEPROM();
                local = layers[0];
            }
            break;

        default:
            if (confirma && !switch_alarme)
            {
                local = layers[0];
            }

            break;
        }
    }
    else if (local == layers[5])
    {

        switch (selection)
        {
        case 31:
            if (confirma && !switch_alarme)
            {
                alarmeDuracaoTemporario += 1;
                if (alarmeDuracaoTemporario > 15)
                {
                    alarmeDuracaoTemporario = 1;
                }
            }
            break;
        case 32:
            if (confirma && !switch_alarme)
            {
                alarmeDuracao = alarmeDuracaoTemporario;
                EEPROM.write(durationPointer, alarmeDuracaoTemporario);
                local = layers[0];
            }
            break;
        default:
            if (confirma && !switch_alarme)
            {
                alarmeDuracaoTemporario = alarmeDuracao;
                local = layers[0];
            }
            break;
        }
    }

    if (selection < 3)
    {
        if (confirma && !switch_alarme)
        {
            local = layers[selection + 1];
        }
    }
    else
    {
        if (confirma && !switch_alarme)
        {
            if (local != layers[4])
            {
                local = layers[0];
            }
            if (selection == 30 && !switch_alarme)
            {
                local = layers[5];
            }
        }
    }
    selecChanger();
}

BlinkLCD TopRowLCD;
BlinkLCD BottomRowLCD;
BlinkLCD BottomRowLCDtext;

void renderUI() {
    int data[7];
    getRTC(data);

    if (local == layers[0]) // layer 0  menu
    {
        Mostrarelogio();

       
        if (alarmandoManual)
        {
            lcd.setCursor(5, 1);
            lcd.print("Alarme");
        }
        else
        {

            switch (selection)
            {
            case 30:
                lcd.setCursor(3, 1);
                lcd.print("Configurar");
                break;

            default:
                lcd.setCursor(2, 1);
                lcd.print("Ver Horarios");
                break;
            }
        }
    }

    else if (local == layers[1]) // layer 1 turnos
    {
        lcd.setCursor(5, 0);
        lcd.print("Turno?");

        lcd.setCursor(5, 1);
        switch (selection)
        {
        case 1:
            lcd.print("Manha");
            break;
        case 2:
            lcd.print("Tarde");
            break;
        case 3:
            lcd.print("Voltar");
        }
    }

    else if (local == layers[2]) // layer 2 turno manhã
    {
        lcd.setCursor(1, 0);
        lcd.print("Horarios Manha");

        if (selection < 12)
        {
            lcd.setCursor(0, 1);
            lcd.print(selection - 3);
            lcd.setCursor(6, 1);
            if (horarios[selection - 4][0] < 10)
            {
                lcd.print("0");
            }
            lcd.print(horarios[selection - 4][0]);
            lcd.print(":");
            if (horarios[selection - 4][1] < 10)
            {
                lcd.print("0");
            }
            lcd.print(horarios[selection - 4][1]);
            lcd.print("      ");
        }
        else
        {
            lcd.setCursor(5, 1);
            lcd.print("Voltar");
        }
    }

    else if (local == layers[3]) // layer 3 turno tarde
    {

        lcd.setCursor(1, 0);
        lcd.print("Horarios Tarde");

        if (selection < 19)
        {
            lcd.setCursor(0, 1);
            lcd.print(selection - 12);
            lcd.setCursor(6, 1);
            if (horarios[selection - 5][0] < 10)
            {
                lcd.print("0");
            }
            lcd.print(horarios[selection - 5][0]);
            lcd.print(":");
            if (horarios[selection - 5][1] < 10)
            {
                lcd.print("0");
            }
            lcd.print(horarios[selection - 5][1]);
            lcd.print("      ");
        }
        else
        {
            lcd.setCursor(5, 1);
            lcd.print("Voltar");
        }
    }
    else if (local == layers[4])
    {

        if (selection < 24)
        {
            lcd.setCursor(6, 0);
            if (hora < 10)
            {
                lcd.print("0");
            }
            lcd.print(hora);
            lcd.print(":");
            if (minuto < 10)
            {
                lcd.print("0");
            }
            lcd.print(minuto);
            switch (selection)
            {
            case 20:
                lcd.setCursor(6, 1);
                BottomRowLCDtext.SetText("  ");
                BottomRowLCDtext.SetStartPosition(22);
                BottomRowLCDtext.SetLength(2);
                BottomRowLCDtext.SetBlinkRate(500);
                BottomRowLCDtext.Update();
                break;
            case 21:
                lcd.setCursor(9, 1);
                BottomRowLCDtext.SetText("  ");
                BottomRowLCDtext.SetStartPosition(25);
                BottomRowLCDtext.SetLength(2);
                BottomRowLCDtext.SetBlinkRate(500);
                BottomRowLCDtext.Update();
                break;
            case 22:
                lcd.setCursor(4, 1);
                lcd.print("Confirmar");
                break;

            case 23:
                lcd.setCursor(4, 1);
                lcd.print("Cancelar");
                break;
            }
        }
    }
    else if (local == layers[5])
    {
        lcd.setCursor(1, 0);
        lcd.print("Periodo em Seg");
        switch (selection)
        {
        case 31:
            lcd.setCursor(2, 1);
            lcd.print(alarmeDuracaoTemporario);
            lcd.print(" ");
            lcd.print("Segundos");
            break;
        case 32:
            lcd.setCursor(4, 1);
            lcd.print("Confirmar");
            break;

        default:
            lcd.setCursor(4, 1);
            lcd.print("Cancelar");
            break;
        }
    }
}

class BlinkLCD
{

private:
    byte m_blinkState, m_startPosition, m_dataLength;
    unsigned long m_dataToBlink, m_previousMillis, m_blinkRate;
    char m_text[16 + 1];

public:
    BlinkLCD() : m_blinkState(0), m_startPosition(0), m_dataLength(0),
                 m_dataToBlink(0), m_previousMillis(0), m_blinkRate(500),
                 m_text() {}

    void Update()
    {

        unsigned long m_currentMillis = millis();

        if (m_currentMillis - m_previousMillis >= m_blinkRate)
        {

            byte m_row = 0;
            m_blinkState = !m_blinkState;
            m_previousMillis = m_currentMillis;

            if (m_startPosition > 15)
            {
                m_startPosition -= 16;
                m_row = 1;
            }

            lcd.setCursor(m_startPosition, m_row);

            if (m_blinkState)
            {
                if ((unsigned)strlen(m_text) > 0)
                {
                    lcd.print(m_text);
                }
                else
                {
                    lcd.print(m_dataToBlink);
                }
            }
            else
            {
                for (byte i = 0; i < m_dataLength; i++)
                {
                    lcd.write(byte(0));
                }
            }
        }
    }

    void SetBlinkRate(unsigned long blinkRate)
    {
        m_blinkRate = blinkRate;
    }

    void SetNumber(unsigned long dataToBlink)
    {
        m_dataToBlink = dataToBlink;
    }

    void SetLength(byte dataLength)
    {
        m_dataLength = dataLength;
    }

    // LCD top row, columns 0 - 15
    // LCD Bottom row, 16 - 31
    void SetStartPosition(byte startPosition)
    {
        m_startPosition = startPosition;
    }

    void SetText(char text[])
    {
        strcpy(m_text, text);
    }
};

// Other helper functions for specific functionalities