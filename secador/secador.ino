#define sensor_PIR A2
#define led A3
#define rele A4

uint64_t millis64() {
    static uint32_t low32, high32;
    uint32_t new_low32 = millis();
    if (new_low32 < low32) high32++;
    low32 = new_low32;
    return (uint64_t) high32 << 32 | low32;
}

unsigned long currentMillis = millis64();

unsigned long previousMillis;

unsigned long second = 1000;

bool checar_um_segundo = currentMillis - previousMillis >= second;
bool checar_metade_segundo = currentMillis - previousMillis >= second/2;
bool checar_decimo_segundo = currentMillis - previousMillis >= second/10;

bool desligado = false;

bool flag = false;

void desligaMotor(){
  for(int r=0;r<10;r++)
  {
    delay(100);
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);
  }
  
  
  digitalWrite(rele, LOW);
  Serial.println("Desligar");
  

}

void setup(){

  pinMode(sensor_PIR, INPUT);
  pinMode(led, OUTPUT);
  pinMode(rele, OUTPUT);
  Serial.begin(9600);

}

void loop(){

  unsigned long currentMillis = millis64();

  int PIR;
  int count = 0;
  
  digitalWrite(led, HIGH);

  Serial.println(analogRead(sensor_PIR));
  
  PIR = analogRead(sensor_PIR);
  
  if(PIR <= 970 && analogRead(sensor_PIR) <= 970 && flag && currentMillis - previousMillis >= 2000) 
  {
    Serial.println("ligou");
    PIR = 0;
    

    for(count = 0; count < 6; count++)
    {
      digitalWrite(rele, HIGH);
      
      delay(500);
      digitalWrite(led, HIGH);
      delay(500);
      digitalWrite(led, LOW);

      if (analogRead(sensor_PIR) >= 970){
        count = 6;
        digitalWrite(rele, LOW);
        desligado = true;
      }
      Serial.println(count);
    }
    if(desligado == false){
      desligaMotor();
    }
    previousMillis = currentMillis;
    flag = false;
    desligado = false;
  }

  if(currentMillis - previousMillis >= 2000){
    previousMillis = currentMillis;
    flag = true;
  }

}
  
 
  
