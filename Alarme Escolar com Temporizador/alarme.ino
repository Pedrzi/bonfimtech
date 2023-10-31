// Inclusão de bibliotecas
#include "functions.h"
#include <LiquidCrystal.h>  
#include <EEPROM.h>
#include "Wire.h"


byte zero = 0x00;
LiquidCrystal lcd(9,8,7,6,5,4); // pins do LCD

char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};

int selection         =  0; // seleção atual do menu
int local             =  0; // layer atual do menu
int pointer           =  0; // índice atual da matriz de horários
int layerlimit        =  0;
int horarios[][2]     = {
{7,   0}, {7,  50},
{8,  40}, {9,  30},
{9,  50}, {10, 40},
{11, 30}, {12, 10}
};

long previousMillis = 0;
const long second = 1000;

void setup() 
{   
    Serial.begin(9600);
    Wire.begin();
    
    StartLCD();
    Serial.println("LCD started");

    StartEEPROM();
    Serial.println("EEPROM started");

    StartButtons();
    Serial.println("Buttons started");

    //SelecionaDataeHora();

    delay(100);
}

void loop() 
{   
    unsigned long currentMillis = millis64();

    int change   = digitalRead(mudar);
    int confirm  = digitalRead(confirmar);

    Menu();

    if(digitalRead(mudar) == LOW && change == LOW && currentMillis - previousMillis > 300)
    {
        previousMillis = currentMillis;

        Serial.println("Mudar!");

        switch (local)
        {
        case 0: // layer 0
            // nothing
            break;
        
        default:
            selection++;

            if(selection > layerlimit)
            {
                selection = 0;
            }
            break;
        }
        
            

        lcd.clear();
        
        
    }

    if(digitalRead(confirmar) == LOW && confirm == LOW && currentMillis - previousMillis > 300)
    {
        previousMillis = currentMillis;

        Serial.println("Confirmar!");

        switch (local)
        {
        case 0: //layer 0
            local = 1;
            selection = 0;
            break;
        
        case 1: //layer 1

            switch (selection)
            {
            case 0: // turno manha
                local = 2;
                selection = 0;
                break;
            
            case 1: // turno tarde
                local = 3;
                selection = 0;
                break;

            case 2: // voltar
                local = 0;
                selection = 0;
                break;
            }
            
            break;

        case 2: // turno manha

            switch (selection)
            {
            case 0:
                break;
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
                break;
            case 8:
                local = 1;
                selection = 0;
                break;
            }
            break;
        case 3: //turno tarde

            switch (selection)
            {
                case 0:
                    
                    break;
                
                case 1:
                    local = 1;
                    selection = 0;
                    break;
            }
            break;

        }
        

        lcd.clear();
    }
    
    Checarhora();    
    
}

void Menu()
{
    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(zero);
    Wire.endTransmission();
    Wire.requestFrom(DS1307_ADDRESS, 7);
    int segundos = ConverteparaDecimal(Wire.read());
    int minutos = ConverteparaDecimal(Wire.read());
    int horas = ConverteparaDecimal(Wire.read() & 0b111111); 
    int diadasemana = ConverteparaDecimal(Wire.read()); 
    int diadomes = ConverteparaDecimal(Wire.read());
    int mes = ConverteparaDecimal(Wire.read());
    int ano = ConverteparaDecimal(Wire.read());

    if(local == 0) // layer 0 
    {
        layerlimit = 0;
        Mostrarelogio();

        lcd.setCursor(5,1);
        lcd.print("listar");
    }

    else if(local == 1) // layer 1
    {
        layerlimit = 2;

        lcd.setCursor(5, 0);
        lcd.print("Turno?");

        lcd.setCursor(0, 1);

        switch (selection) 
        {
            case 0:
                lcd.print("Manha");
                break;
            case 1:
                lcd.print("Tarde");
                break;
            case 2:
                lcd.print("Voltar");
        }
        
    }

    else if(local == 2) // layer 2 turno manhã
    {
        layerlimit = 8;
        lcd.setCursor(1,0);
        lcd.print("Horarios Manha");

        switch(selection)
        {
        case 0:
            
            lcd.setCursor(6,1);    
            lcd.print("0");    
            lcd.print(horarios[0][0]);
            lcd.print(":");
            lcd.print("0");
            lcd.print(horarios[0][1]);

            break;
        
        case 1:

            lcd.setCursor(6,1);    
            lcd.print("0");    
            lcd.print(horarios[1][0]);
            lcd.print(":");
            lcd.print(horarios[1][1]);
            

            break;
        case 2:

            lcd.setCursor(6,1);    
            lcd.print("0");    
            lcd.print(horarios[2][0]);
            lcd.print(":");
            lcd.print(horarios[2][1]);

            break;

        case 3:

            lcd.setCursor(6,1);    
            lcd.print("0");    
            lcd.print(horarios[3][0]);
            lcd.print(":");
            lcd.print(horarios[3][1]);

            break;

        case 4:

            lcd.setCursor(6,1);    
            lcd.print("0");
            lcd.print(horarios[4][0]);
            lcd.print(":");
            lcd.print(horarios[4][1]);

            break;

        case 5:

            lcd.setCursor(6,1);    
   
            lcd.print(horarios[5][0]);
            lcd.print(":");
            lcd.print(horarios[5][1]);

            break;

        case 6:

            lcd.setCursor(6,1);    
              
            lcd.print(horarios[6][0]);
            lcd.print(":");
            lcd.print(horarios[6][1]);

            break;

        case 7:

            lcd.setCursor(6,1);    
   
            lcd.print(horarios[7][0]);
            lcd.print(":");
            lcd.print(horarios[7][1]);

            break;

        case 8:

            lcd.setCursor(5,1);
            lcd.print("Voltar");

            break;
        }

    }

    else if(local == 3) // layer 3 turno tarde
    {

        lcd.setCursor(0,0);
        lcd.print("tarde:");

        switch(selection)
        {
        case 0:
            lcd.setCursor(11,0);
            if (horas < 10)
            {
                lcd.print("0");
            }
            lcd.print(horas);
            lcd.print(":");
            if (minutos < 10)
            {
                lcd.print("0");
            }
            lcd.print(minutos);
            lcd.print(":");

            break;
        
        case 1:

            lcd.setCursor(5,1);
            lcd.print("Voltar");

            break;
        }

    }
    
    
}
