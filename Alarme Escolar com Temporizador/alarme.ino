// Inclusão de bibliotecas
#include "functions.h"
#include <LiquidCrystal.h>  
#include <EEPROM.h>
#include "Wire.h"

char   layers[][16] = {"menu", "turnos", "manha", "tarde"};
byte zero = 0x00;
LiquidCrystal lcd(9,8,7,6,5,4); // pins do LCD

char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};

int selection         =  0; // seleção atual do Render
char *local           = "menu"; // layer atual do Render
int pointer           =  0; // índice atual da matriz de horários
int layerlimit        =  0;
int horarios[][2]     = {
{7,   0}, {7,  50},
{8,  40}, {9,  30},
{9,  50}, {10, 40},
{11, 30}, {12, 10},
{13,  0}, {13, 40},
{14, 20}, {15,  0},
{15, 20}, {16, 0},
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
    unsigned long currentTime = millis();
    int muda = !digitalRead(mudar);
    bool confirma = false;
    if(!digitalRead(confirmar) && currentTime - previousMillis > 200)
    {
        previousMillis = currentTime;
        confirma = true;
        lcd.clear();
    }
 
    if(local == "menu")
    {
        
    }
    else if (local == "turnos")
    {
        
    }
    else if (local == "manha")
    {
        
    }
    else if (local == "tarde")
    {
        
    }
    else
    {
        
    }
  // Check the value of 'selection' and change 'local' accordingly
    switch (selection) {
        case 0: // Handle selection case 0
            if (confirma) {
                local = "turnos"; // Set local to "Render"
            }
        break;
        case 1: // Handle selection case 1
            if (confirma) {
                local = "manha"; // Set local to "turnos"
            }
        break;
        case 2: // Handle selection case 2
            if (confirma) {
                local = "tarde"; // Set local to "manha"
            }
        break;
        default:
            if (confirma) {
                local = "menu"; // Set local to "tarde"
            }
        break;
    }

    if(muda && currentTime-previousMillis > 200)
    {
        previousMillis = currentTime;

        selection++;
        lcd.clear();
    }

    // Render the 'local'
    selecChanger();
    Render();
    
    Serial.println(selection);
    
}
void Render()
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

    if(local == "menu") // layer 0 
    {
        Mostrarelogio();

        lcd.setCursor(5,1);
        lcd.print("listar");
    }

    else if(local == "turnos") // layer 1
    {
        lcd.setCursor(5, 0);
        lcd.print("Turno?");

        switch (selection) 
        {
            case 1:
                lcd.setCursor(5,1);
                lcd.print("Manha");
                break;
            case 2:
            lcd.setCursor(5,1);
                lcd.print("Tarde");
                break;
            case 3:
            lcd.setCursor(5,1);
                lcd.print("Voltar");
        }
        
    }

    else if(local == "manha") // layer 2 turno manhã
    {
        lcd.setCursor(1,0);
        lcd.print("Horarios Manha");

        switch(selection)
        {
        case 12:

            lcd.setCursor(5,1);
            lcd.print("Voltar");

            break;
        default:

            lcd.setCursor(0,1);
            lcd.print(selection-3);
            lcd.setCursor(6,1);
            if(horarios[selection-4][0] < 10){
                lcd.print("0");
            }
            lcd.print(horarios[selection-4][0]);
            lcd.print(":");
            if(horarios[selection-4][1] < 10){
                lcd.print("0");
            }
            lcd.print(horarios[selection-4][1]);
            lcd.print("      ");
            
            break;
        }

    }

    else if(local == "tarde") // layer 3 turno tarde
    {

        lcd.setCursor(1,0);
        lcd.print("Horarios Tarde");

        switch(selection)
        {
        case 18:

            lcd.setCursor(5,1);
            lcd.print("Voltar");

            break;
        default:
            lcd.setCursor(0,1);
            lcd.print(selection-11);
            lcd.setCursor(6,1);
            if(horarios[selection-4][0] < 10){
                lcd.print("0");
            }
            lcd.print(horarios[selection-4][0]);
            lcd.print(":");
            if(horarios[selection-4][1] < 10){
                lcd.print("0");
            }
            lcd.print(horarios[selection-4][1]);
            lcd.print("      ");
            
            break;
        }
    }
    
    
}

