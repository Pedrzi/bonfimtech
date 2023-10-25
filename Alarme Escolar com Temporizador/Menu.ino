// Inclusão de bibliotecas
#include <LiquidCrystal.h>  
#include <EEPROM.h>

#define mudar     A3 // Botão de mudar
#define confirmar A2 // Botão de confirmar
#define numRows   64 // Número de linhas da matriz de horários
#define numCols    2 // número de colunas da matriz de horáriios
#define Contrast 130 // contraste da tela LCD
#define pointerIndex 98 // Índice do pontador

LiquidCrystal lcd(9,8,7,6,5,4); // pins do LCD

int selection         =  0; // seleção atual do menu
int local             =  0; // layer atual do menu
int pointer           =  0; // índice atual da matriz de horários
int horarios[64][2]   = {}; // Matriz de horários

long previousMillis = 0;
const long second = 1000;

void setup() 
{   
    Serial.begin(9600);
    
    startLCD();
    Serial.println("LCD started");

    startEEPROM();
    Serial.println("EEPROM started");

    startButtons();
    Serial.println("Buttons started");

    delay(100);
}

void loop() 
{   
    unsigned long currentMillis = millis64();

    int change   = digitalRead(mudar);
    int confirm  = digitalRead(confirmar);

    menu();
    

    if(digitalRead(mudar) == LOW && change == LOW && currentMillis - previousMillis > 300)
    {
        previousMillis = currentMillis;

        Serial.println("Mudar!");

        switch (local)
        {
        case 0: // layer 0
            // nothing
            break;
        
        case 1: // layer 1
            selection++;

            if(selection > 2)
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
                
                break;
            
            case 1: // turno tarde
                break;

            case 2: // voltar
                local = 0;
                selection = 0;
                break;
            }
            
            break;
        }
        

        lcd.clear();
    }
    
}

void menu()
{

    if(local == 0)
    {
        lcd.setCursor(6,0);
        lcd.print("Menu");

        lcd.setCursor(5,1);
        lcd.print("listar");
    }

    if(local == 1)
    {
        lcd.setCursor(5, 0);
        lcd.print("Turno?");

        lcd.setCursor(0, 1);

        switch (selection) {
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
    
    
}

uint64_t millis64() {
    static uint32_t low32, high32;
    uint32_t new_low32 = millis();
    if (new_low32 < low32) high32++;
    low32 = new_low32;
    return (uint64_t) high32 << 32 | low32;
}

void startButtons()
{
    pinMode(mudar, INPUT_PULLUP);
    pinMode(confirmar, INPUT_PULLUP);
}

void startEEPROM()
{
    if (EEPROM.read(pointerIndex) > 64)
    {
        EEPROM.write(pointerIndex, 0);
    }

    pointer = EEPROM.read(pointerIndex);

    if(pointer > 0)
    {
        readArrayFromEEPROM();
    }

    Serial.print("Pointer index is at: ");
    Serial.println(pointer);
}

void loadingAnim()
{
    int dots = 1;
    for(int i = 0; i < 7; i++){
        lcd.clear();
        if(dots > 3){
        dots = 1;
        }
        switch (dots)
        {
        case 1:
        lcd.print("Loading.");
        break;
        case 2:
        lcd.print("Loading..");
        break;
        case 3:
        lcd.print("Loading...");
        break;
        }
        dots++;
        delay(500);
        
    }
}

void startLCD()
{
    analogWrite(3, Contrast);

    lcd.begin(16, 2);

    lcd.setCursor(0,0);
    lcd.print("Hello!");
    delay(1000);

    loadingAnim();
    lcd.clear();
}

void saveArrayToEEPROM() 
{
  int addr = 0;
  for (int row = 0; row < pointer; row++) {
    for (int col = 0; col < numCols; col++) {
      EEPROM.update(addr, horarios[row][col]);
      addr++;
    }
  }
}

void readArrayFromEEPROM() 
{
  int addr = 0;
  for (int row = 0; row < pointer; row++) {
    for (int col = 0; col < numCols; col++) {
      horarios[row][col] = EEPROM.read(addr);
      addr++;
    }
  }
}
