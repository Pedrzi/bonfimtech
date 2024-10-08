/*
Funções da classe usada para piscar texto na tela do lcd
*/
#include "BlinkLCD.h"
#include "jobs.h"

extern LiquidCrystal lcd;

void BlinkLCD::Update()
{

    unsigned long m_currentMillis = millis();

    if (m_currentMillis - m_previousMillis >= m_blinkRate)
    {

        int m_row = 0;
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
            for (int i = 0; i < m_dataLength; i++)
            {
                lcd.print(" ");
            }
        }
    }
}

void BlinkLCD::SetBlinkRate(unsigned long blinkRate)
{
    m_blinkRate = blinkRate;
}

void BlinkLCD::SetNumber(unsigned long dataToBlink)
{
    m_dataToBlink = dataToBlink;
}

void BlinkLCD::SetLength(int dataLength)
{
    m_dataLength = dataLength;
}

// LCD top row, columns 0 - 15
// LCD Bottom row, 16 - 31
void BlinkLCD::SetStartPosition(int startPosition)
{
    m_startPosition = startPosition;
}

void BlinkLCD::SetText(char text[])
{
    strcpy(m_text, text);
}