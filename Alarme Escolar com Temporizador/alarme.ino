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
int horarios[][2]   = {}; // Matriz de horários

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
                local = 1;
                selection = 0;
                break;
            }

        }
        

        lcd.clear();
    }
    
    if(pointer > 0){
        // Checarhora(pointer, horarios[][2]);
    }
    

}

void Menu()
{

    if(local == 0) // layer 0 
    {
        Mostrarelogio();

        lcd.setCursor(5,1);
        lcd.print("listar");
    }

    if(local == 1) // layer 1
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

    if(local == 2) // layer 2 turno manhã
    {
        layerlimit = 1;

        lcd.setCursor(0,0);
        lcd.print("Manha:");

        switch(selection)
        {
        case 0:
            
            lcd.setCursor(12,0);
            lcd.print("hora");

            break;
        
        case 1:

            lcd.setCursor(5,1);
            lcd.print("Voltar");

            break;
        }

    }
    
    
}
