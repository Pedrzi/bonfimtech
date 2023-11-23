int conta1;
int conta2;

//Botões que incrementam os pontos
int BtnPlacarVermelho = 0;
int BtnPlacarAzul = 1;

//Botões que acionam os leds
int BtnVermelho = A2;
int BtnAzul = A3;

//Portas que ligam os leds
int LedVermelhoRele = A5;
int LedAzulRele = A4;

const byte pinosLEDs1[] = { 2, 3, 4, 5, 6, 7, 8 };

byte displaySeteSeg1[10][7] = {

  { 1, 1, 1, 1, 1, 1, 0 },  // DIGITO 0
  { 0, 1, 1, 0, 0, 0, 0 },  // DIGITO 1
  { 1, 1, 0, 1, 1, 0, 1 },  // DIGITO 2
  { 1, 1, 1, 1, 0, 0, 1 },  // DIGITO 3
  { 0, 1, 1, 0, 0, 1, 1 },  // DIGITO 4
  { 1, 0, 1, 1, 0, 1, 1 },  // DIGITO 5
  { 1, 0, 1, 1, 1, 1, 1 },  // DIGITO 6
  { 1, 1, 1, 0, 0, 0, 0 },  // DIGITO 7
  { 1, 1, 1, 1, 1, 1, 1 },  // DIGITO 8
  { 1, 1, 1, 1, 0, 1, 1 }   // DIGITO 9

};
// Pinos digitais diretamente associados aos LEDs
const byte pinosLEDs2[] = { 9, 10, 11, 12, 13, A0, A1 };

byte displaySeteSeg2[10][7] = {

  { 1, 1, 1, 1, 1, 1, 0 },  // DIGITO 0
  { 0, 1, 1, 0, 0, 0, 0 },  // DIGITO 1
  { 1, 1, 0, 1, 1, 0, 1 },  // DIGITO 2
  { 1, 1, 1, 1, 0, 0, 1 },  // DIGITO 3
  { 0, 1, 1, 0, 0, 1, 1 },  // DIGITO 4
  { 1, 0, 1, 1, 0, 1, 1 },  // DIGITO 5
  { 1, 0, 1, 1, 1, 1, 1 },  // DIGITO 6
  { 1, 1, 1, 0, 0, 0, 0 },  // DIGITO 7
  { 1, 1, 1, 1, 1, 1, 1 },  // DIGITO 8
  { 1, 1, 1, 1, 0, 1, 1 }   // DIGITO 9

};



void setup() {
  Serial.begin(9600);

  //Definindo pinos como saída
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);

  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);


  pinMode(BtnPlacarVermelho, INPUT_PULLUP);
  pinMode(BtnPlacarAzul, INPUT_PULLUP);

  pinMode(BtnVermelho, INPUT_PULLUP);
  pinMode(BtnAzul, INPUT_PULLUP);

  pinMode(LedVermelhoRele, OUTPUT);
  pinMode(LedAzulRele, OUTPUT);

  //inicializa display com número 0
  conta1 = 0;
  conta2 = 0;
  ligaSegmentosDisplay1(conta1);
  ligaSegmentosDisplay2(conta2);
}

void loop() {

  if (!digitalRead(BtnVermelho) && digitalRead(BtnAzul)) {
    digitalWrite(LedAzulRele, LOW);
    digitalWrite(LedVermelhoRele, HIGH);
    Serial.println("BtnAzul");
    delay(2000);
  } else {
    digitalWrite(LedVermelhoRele, LOW);
  }

  if (digitalRead(BtnVermelho) && !digitalRead(BtnAzul)) {
    digitalWrite(LedVermelhoRele, LOW);
    digitalWrite(LedAzulRele, HIGH);
    Serial.println("BtnVermelho");
    delay(2000);
  } else {
    digitalWrite(LedAzulRele, LOW);
  }


  if (digitalRead(BtnPlacarVermelho) == LOW) {  // Se o botão for pressionado, soma +1 no display
    delay(500);
    conta1++;  // Incremento

    if (conta1 == 10) {  // Se passar de 9, reseta o display para 0
      conta1 = 0;
    }

    ligaSegmentosDisplay1(conta1);
  }

  if (digitalRead(BtnPlacarAzul) == LOW) {  // Se o botão for pressionado, soma +1 no display
    delay(500);
    conta2++;  // Incremento

    if (conta2 == 10) {  // Se passar de 9, reseta o display para 0
      conta2 = 0;
    }

    ligaSegmentosDisplay2(conta2);
  }

    if (digitalRead(BtnPlacarVermelho) == LOW && digitalRead(BtnPlacarAzul) == LOW) {
    conta1 = 0;
    conta2 = 0;
    ligaSegmentosDisplay1(conta1);
    ligaSegmentosDisplay2(conta2);
    delay(1500);
  }
}

void ligaSegmentosDisplay1(byte digito) {

  // Ativa os LEDs correspondentes ao dígito
  for (byte contadorSegmentos = 0; contadorSegmentos < 7; ++contadorSegmentos) {
    byte pino = pinosLEDs1[contadorSegmentos];
    byte valor = displaySeteSeg1[digito][contadorSegmentos];
    digitalWrite(pino, valor);
  }
}

void ligaSegmentosDisplay2(byte digito) {

  // Ativa os LEDs correspondentes ao dígito
  for (byte contadorSegmentos = 0; contadorSegmentos < 7; ++contadorSegmentos) {
    byte pino = pinosLEDs2[contadorSegmentos];
    byte valor = displaySeteSeg2[digito][contadorSegmentos];
    digitalWrite(pino, valor);
  }
}