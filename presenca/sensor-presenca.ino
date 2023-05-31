#define PIN_SENSOR 4

void setup() {

  //abaixo vamos confirar cada um dos pinos como entrada ou saída de dados
  pinMode(PIN_SENSOR, INPUT);
}

void loop() { 

  //faz a leitura do sensor de presença (retorna HIGH ou LOW)
  int sinal = digitalRead(PIN_SENSOR); 

  //HIGH : movimento detectado
  if(sinal == HIGH){
    //aciona o Buzzer
    Serial.print("high: ");
  }
  //LOW : nenhum movimento detectado
  else{
    //desativa o buzzer
    Serial.print("low: ");
  }
}
