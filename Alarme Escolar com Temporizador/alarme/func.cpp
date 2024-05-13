#include "func.h"

extern byte barra[8];

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
int layers[7][4] = {{1, 4, 1, 4}, {2, 3, 0, 0}, {99, 99, 99, 99}, {99, 99, 99, 99}, {5, 6, 0, 0}, {99, 99, 99, 99}, {99, 99, 99, 99}};
char layersName[7][17] = 
{"      menu      ", "     config     ", "   mudar hora   ", "  mudar duracao ",
"  ver horarios  ", "horarios tarde", "horarios manha"};

int currentLayer = 0;

int currentSelection = 0;
int selectionIndex = 0;

const int buttonWaitTime    = 200;

struct boolPair;

LiquidCrystal lcd(9, 8, 7, 6, 5, 4);

uint64_t moverTimer = 0;
uint64_t validarTimer = 0;
// Checa se algum botão foi apertado e retorna esse valor
boolPair checarBotoes()
{
    bool mov = false;
    bool val = false;
    if(millis64() - moverTimer > buttonWaitTime && !digitalRead(12))
    {
        moverTimer = millis64();
        mov = true;
    
    }

    if(millis64() - validarTimer > buttonWaitTime && !digitalRead(11))
    {
        validarTimer = millis64();
        val = true;
    }

    return {val, mov};
}


void specialLayers()
{
    
}


void handleLayers()
{
    boolPair botao = checarBotoes();
    bool val = botao.validar;
    bool mov = botao.mover;

    if(val) currentLayer = currentSelection;
    if(mov) selectionIndex++;
    if(selectionIndex>3) selectionIndex = 0;
    currentSelection = layers[currentLayer][selectionIndex];
    if(selectionIndex == 3 && currentSelection == 0) selectionIndex = 0;
    
}


uint64_t renderTimer = 0;
void render()
{
    if(millis64() - renderTimer < 100)
    {
        return;
    }
    renderTimer = millis64();

    lcd.clear();

    if(currentLayer == 0)
    {
        lcd.setCursor(0,0);
        Mostrarelogio();
    }

    lcd.setCursor(0,1);
    lcd.print(layersName[currentSelection]);
}


// Função que da print no painel lcd o horário atual
void Mostrarelogio()
{
    int data[7];
    getRTC(data);
    // Imprime mensagem na primeira linha do display
    // Mostra a hora atual no display
    lcd.setCursor(4, 0);
    if (data[2] < 10)
    {lcd.print("0");}
    lcd.print(data[2]);
    lcd.print(":");
    if (data[1] < 10)
    {lcd.print("0");}
    lcd.print(data[1]);
    lcd.print(":");
    if (data[0] < 10)
    {lcd.print("0");}
    lcd.print(data[0]);

}
