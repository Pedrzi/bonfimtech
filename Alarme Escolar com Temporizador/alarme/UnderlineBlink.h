#ifndef UnderlineBlink_H
#define UnderlineBlink_H

class UnderlineBlink
{

private:
    int m_blinkState, m_startPosition, m_dataLength;
    unsigned long m_dataToBlink, m_previousMillis, m_blinkRate;
    char m_text[16 + 1];


public:
    UnderlineBlink() : m_blinkState(0), m_startPosition(0), m_dataLength(0),
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