#include "func.h"
#include "jobs.h"
#include "BlinkLCD.h"
#include "UnderlineBlink.h"

/*
Essa variável controla a quantidade de layers
e também a quantidade de opções selecionáveis
cada layer possui. Cada opção selecionável
representa um índice diferente.

`layers[quant. layers][opções]`

0: Menu
1: Configurações
2: Mudar horário
3: Mudar duração do alarme
4: Ver horários
5: Horários da manhã
6: Horários da tarde
99: quantidade de opções dinâmicas
*/
int layers[8][4] = {{1, 4, 7, 4}, {2, 3, 0, 0}, {99, 99, 99, 99}, {77, 77, 77, 77}, {5, 6, 0, 0}, {88, 88, 88, 88}, {88, 88, 88, 88}, {66, 66, 66, 66}};
char layersName[8][17] =
    {"      Menu      ", "   Configurar   ", " Redefinir hora ", " Tempo de toque ",
     " Listar alarmes ", "     Manha     ", "     Tarde     ", "     Tocar     "};
const char dias[7][4] = 
    {"Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sab"};
int currentLayer = 0;
int currentSelection = 0;
int selectionIndex = 0;
extern int tempoToque;

// Variável que contém todos os horários padrões do alarme
int horarios[HORARIOSIZE][2] = {
    {7, 0}, {7, 50}, {8, 40}, {9, 30}, {9, 50}, {10, 40}, {11, 30}, {12, 10}, {13, 0}, {13, 40}, {14, 20}, {15, 0}, {15, 20}, {16, 0}
    };

struct boolPair;

//declarado no arquivo jobs.cpp
extern LiquidCrystal lcd;

BlinkLCD BottomRowLCDtext;
UnderlineBlink Underline;

// Função usada para reiniciar o alarme 
void (*resetFunc)(void) = 0;

void handleLayers()
{
    boolPair botao = checarBotoes();
    bool val = botao.validar;
    bool mov = botao.mover;

    currentSelection = layers[currentLayer][selectionIndex];
    if (currentSelection == 88)
    {
        mostrarHorarios(val, mov, currentLayer);
        return;
    }
    if (currentSelection == 99)
    {
        editor(val, mov, currentLayer);
        return;
    }
    if (currentSelection == 77)
    {
        mudarTempoDeToque(val, mov);
        return;
    }
    if (currentSelection == 66)
    {
        testarAlarme(val);
        return;
    }
    if (val)
    {
        currentLayer = currentSelection;
        selectionIndex = 0;
    }
    if (mov)
        selectionIndex++;
    if (selectionIndex > 3)
        selectionIndex = 0;
    if (selectionIndex == 3 && currentSelection == 0)
        selectionIndex = 0;
    renderMenu(layersName[currentLayer]);
}

int tempHora = 0;
int tempMinutosU = 0;
int tempMinutosD = 0;
int tempDia = 0;
bool edit = false;
int horarioSelect = 0;
void mostrarHorarios(bool vali, bool mov, int current)
{
    horarioSelect = horarioSelect > HORARIOSIZE ? 0 : horarioSelect;

    if (mov && !edit)
    {
        horarioSelect++;
    }
        
    if (current == 5)
    {

        horarioSelect = horarioSelect > 8 ? 0 : horarioSelect;
        if (horarioSelect == 8 && vali)
        {
            currentLayer = 4;
        }
    }
    else
    {
        horarioSelect = horarioSelect < 8 ? 8 : horarioSelect;

        if (horarioSelect == HORARIOSIZE && vali)
        {
            currentLayer = 4;
        }
    }

    if (vali || edit)
    {
        numArr digitos = dividirEmDois(horarios[horarioSelect][1]);
        int minU = digitos.num2;
        int minD = digitos.num1;
        if (!edit)
        {
            tempMinutosD = minD;
            tempMinutosU = minU;
            tempHora = horarios[horarioSelect][0];
            vali = !vali;
            edit = true;
        }
        

        editor(vali, mov, currentLayer);
        return;
    }
    renderHorarios();
}

int tempToque = EEPROM.read(ENDERECODETOQUE);
void mudarTempoDeToque(bool v, bool m)
{
    if (m)
        tempToque = (tempToque%LIMITETOQUE)+1;
    if (v)
    {
        EEPROM.write(ENDERECODETOQUE, tempToque);
        resetFunc();
    }
    renderMudarDuracao();
}

int selection = 0;
char tittle[17] = "";
void editor(bool valid, bool move, int current)
{
    
    if (move)
        selection = (selection + 1) % 6;

    if (valid) {
        if (selection == 0)
            tempMinutosU++;
        else if (selection == 1)
            tempMinutosD++;
        else if (selection == 2)
            tempHora++;
        else if (selection == 3)
        {
            if (currentLayer != 2)
            {
              selection++;
              return;
            }
            tempDia++;
        }
        else if (selection == 4)
        {
            if (currentLayer == 2)
            {
                mudarHorario(tempMinutosD * 10 + tempMinutosU, tempHora, tempDia);
                resetFunc();
            }
            horarios[horarioSelect][0] = tempHora;
            horarios[horarioSelect][1] = (tempMinutosD * 10 + tempMinutosU);
            SaveArrayToEEPROM();
            edit = false;
            return;
        }
        else if (selection == 5)
        {
            currentLayer = 0;
            edit = false;
            return;
        }
    }
    tempDia = tempDia > 6 ? 0 : tempDia;
    tempMinutosD = tempMinutosD > 5 ? 0 : tempMinutosD;
    tempMinutosU = tempMinutosU > 9 ? 0 : tempMinutosU;
    tempHora = tempHora > 23 ? 0 : tempHora;
    
    renderEditor();
}

void testarAlarme(bool v)
{
    digitalWrite(RELAY, HIGH);
    lcd.setCursor(0,0);
    lcd.print("     Parar    ");
    lcd.setCursor(0,1);
    lcd.print("                ");
    if(v)
    {
        digitalWrite(RELAY, LOW);
        currentLayer = 0;;
    }
    
}

uint64_t renderTimer = 0;
void renderMenu(char layerTitle[17])
{
    if (millis64() - renderTimer < 100)
        return;
    renderTimer = millis64();

    lcd.clear();
    lcd.setCursor(0, 0);
    if (currentLayer == 0)
    {
        mostrarRelogio();
    }
    else
        lcd.print(layerTitle);

    if (currentSelection != 88)
    {
        lcd.setCursor(0, 1);
        lcd.print(layersName[currentSelection]);
        return;
    }
}

void renderEditor()
{
    if (millis64() - renderTimer < 100)
        return;
    renderTimer = millis64();

    lcd.clear();
    lcd.setCursor(5, 0);
    if (tempHora < 10)
        lcd.print(0);
    lcd.print(int(tempHora));
    lcd.print(":");
    lcd.print(int(tempMinutosD));
    lcd.print(int(tempMinutosU));

    BottomRowLCDtext.SetStartPosition(16);
    BottomRowLCDtext.SetLength(2);
    BottomRowLCDtext.SetBlinkRate(200);
    Underline.SetLength(1);
    Underline.SetBlinkRate(200);

    switch (selection)
    {
    case 0:
        Underline.SetText(" ");
        Underline.SetStartPosition(25);
        Underline.Update();
        break;
    case 1:
        Underline.SetText(" ");
        Underline.SetStartPosition(24);
        Underline.Update();
        break;
    case 2:
        Underline.SetLength(2);
        Underline.SetText("  ");
        Underline.SetStartPosition(21);
        Underline.Update();
        break;
    case 3:
        lcd.setCursor(0, 1);
        BottomRowLCDtext.SetText(dias[tempDia]);
        BottomRowLCDtext.Update();
        break;
    case 4:
        lcd.setCursor(0, 1);
        BottomRowLCDtext.SetText("     salvar     ");
        BottomRowLCDtext.Update();
        break;
    case 5:
        lcd.setCursor(0, 1);
        BottomRowLCDtext.SetText("    cancelar    ");
        BottomRowLCDtext.Update();
        break;
    }
}

void renderHorarios()
{
    if (millis64() - renderTimer < 100)
        return;
    renderTimer = millis64();
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print(layersName[currentLayer]);
    if (currentLayer == 5)
    {

        if (horarioSelect != 8)
        {
            lcd.setCursor(5, 1);
            if (horarios[horarioSelect][0] < 10)
                lcd.print(0);
            lcd.print(horarios[horarioSelect][0]);
            lcd.print(":");
            if (horarios[horarioSelect][1] < 10)
                lcd.print(0);
            lcd.print(horarios[horarioSelect][1]);
            return;
        }
        lcd.setCursor(0, 1);
        lcd.print("     voltar     ");
        return;
    }

    if (horarioSelect != 14)
    {
        lcd.setCursor(5, 1);
        if (horarios[horarioSelect][0] < 10)
            lcd.print(0);
        lcd.print(horarios[horarioSelect][0]);
        lcd.print(":");
        if (horarios[horarioSelect][1] < 10)
            lcd.print(0);
        lcd.print(horarios[horarioSelect][1]);
        return;
    }
    lcd.setCursor(0, 1);
    lcd.print("     voltar     ");
    return;
}

void renderMudarDuracao()
{
    if (millis64() - renderTimer < 100)
        return;
    renderTimer = millis64();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(layersName[currentLayer]);
    lcd.setCursor(7, 1);
    if (tempToque < 10)
        lcd.print(0);
    lcd.print(tempToque);
}
