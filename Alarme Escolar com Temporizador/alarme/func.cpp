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
{"      Menu      ", "   Configurar   ", " Redefinir hora ", " Tempo de toque ",
" Listar alarmes ", "      Tarde     ", "      Manha     "};

int currentLayer = 0;

int currentSelection = 0;
int selectionIndex = 0;

struct boolPair;

LiquidCrystal lcd(9, 8, 7, 6, 5, 4);


void specialLayers(int layer)
{   
    
}


void handleLayers()
{
    boolPair botao = checarBotoes();
    bool val = botao.validar;
    bool mov = botao.mover;

    if(currentSelection == 99)
    {
        specialLayers(currentLayer);
    }

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
    lcd.setCursor(0,0);
    if(currentLayer == 0)
    {
        Mostrarelogio();
    }
    else lcd.print(layersName[currentLayer]); 

    lcd.setCursor(0,1);
    lcd.print(layersName[currentSelection]);
}
