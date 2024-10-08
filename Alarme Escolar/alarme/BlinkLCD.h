/*
Classe usada para fazer texto piscar na tela do lcd
*/
#ifndef BLINKLCD_H
#define BLINKLCD_H

class BlinkLCD
{

private:
    int m_blinkState, m_startPosition, m_dataLength;
    unsigned long m_dataToBlink, m_previousMillis, m_blinkRate;
    char m_text[16 + 1];


public:
    BlinkLCD() : m_blinkState(0), m_startPosition(0), m_dataLength(0),
                 m_dataToBlink(0), m_previousMillis(0), m_blinkRate(500),
                 m_text() {}
    
    void Update();
    void SetBlinkRate(unsigned long blinkRate);
    void SetNumber(unsigned long dataToBlink);
    void SetLength(int dataLength);
    void SetStartPosition(int startPosition);
    void SetText(char text[]);
};



#endif