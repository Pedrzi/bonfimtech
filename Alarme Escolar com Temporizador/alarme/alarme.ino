// Inclusão de bibliotecas
#include "functions.h"
#include <LiquidCrystal.h>  
#include <EEPROM.h>
#include "Wire.h"

char   layers[][16] = {"menu", "turnos", "manha", "tarde", "edição"};
byte zero = 0x00;
LiquidCrystal lcd(9,8,7,6,5,4); // pins do LCD

char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};

int selection         =  0; // seleção atual do Render
char *local           = layers[0]; // layer atual do Render
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
int horarioSize = 16;
int hora = 0;
int minuto = 0;
int index = 0;

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
    //SaveArrayToEEPROM();
    ReadArrayFromEEPROM();
    
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
 
    if(local == layers[0])
    {
        
    }
    else if (local == layers[1])
    {
        
    }
    else if (local == layers[2]) // manha
    {
        if(selection < 12)
        {
            if(confirma)
            {
                // enviar horário para o editor de horári
                hora = horarios[selection-4][0];
                minuto = horarios[selection-4][1];
                index = selection-4;
                local = layers[4];
            }
        }
    }
    else if (local == layers[3]) // tarde
    {
        if(selection < 19)
        { // 16 24 21
            if(confirma)
            {
                // enviar horário para o editor de horário
                hora = horarios[selection-5][0];
                minuto = horarios[selection-5][1];
                index = selection-5;
                local = layers[4];
            }
        }
    }
    else if(local == layers[4]) // editor
    {
        switch (selection)
        {
        case 20:
            if(confirma && currentTime-previousMillis > 200)
            {
                previousMillis = currentTime;

                hora++;
                Serial.println(hora);
                delay(200);
            }
            break;
        case 21:
            if(confirma && currentTime-previousMillis > 200)
            {
                previousMillis = currentTime;

                minuto++;
                Serial.println(minuto);
                delay(200);
            }
            break;
        case 22:
            if(confirma && currentTime-previousMillis > 200)
            {
                previousMillis = currentTime;

                horarios[index][0] = hora;
                horarios[index][1] = minuto;
                SaveArrayToEEPROM();
            }
            break;
        
        default:
            local = layers[0];
            break;
        }
    }
    else
    {
        local = layers[0];
    }


    if(selection < 3)
    {
        if (confirma) 
        {
            local = layers[selection+1]; 
        }
    }
    else
    {
        if (confirma) 
        {
            if(local != layers[4])
            {
                local = layers[0];
            }
             
        }
    }

    // button check
    if(muda && currentTime-previousMillis > 200)
    {
        previousMillis = currentTime;

        selection++;
        lcd.clear();
    }

    // Render the 'local'
    selecChanger();
    Render();
    Checarhora();
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

    if(local == layers[0]) // layer 0  menu
    {
        Mostrarelogio();

        lcd.setCursor(5,1);
        lcd.print("listar");
    }

    else if(local == layers[1]) // layer 1 turnos
    {
        lcd.setCursor(5, 0);
        lcd.print("Turno?");

        lcd.setCursor(5,1);
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

    else if(local == layers[2]) // layer 2 turno manhã
    {
        lcd.setCursor(1,0);
        lcd.print("Horarios Manha");

        if(selection < 12)
        {
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
        }
        else
        {
            lcd.setCursor(5,1);
            lcd.print("Voltar");
        }

    }

    else if(local == layers[3]) // layer 3 turno tarde
    {

        lcd.setCursor(1,0);
        lcd.print("Horarios Tarde");

        if(selection < 19)
        {
            lcd.setCursor(0,1);
            lcd.print(selection-12);
            lcd.setCursor(6,1);
            if(horarios[selection-5][0] < 10){
                lcd.print("0");
            }
            lcd.print(horarios[selection-5][0]);
            lcd.print(":");
            if(horarios[selection-5][1] < 10){
                lcd.print("0");
            }
            lcd.print(horarios[selection-5][1]);
            lcd.print("      ");
        }
        else
        {
            lcd.setCursor(5,1);
            lcd.print("Voltar");
        }
        
    }
    else if(local == layers[4])
    {
        lcd.setCursor(0,0);
        if(hora < 10)
        {
            lcd.print("0");
        }
        lcd.print(hora);
        lcd.print(":");
        if(minuto < 10)
        {
            lcd.print("0");
        }
        lcd.print(minuto);
    }
    
}

void edit()
{
    
    
}