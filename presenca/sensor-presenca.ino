#define LED 5
#define PIN_SENSOR 4

void setup() {

  Serial.begin(9600);  // initialize serial
  //abaixo vamos confirar cada um dos pinos como entrada ou saída de dados
  pinMode(PIN_SENSOR, INPUT);
  pinMode(LED, OUTPUT);
}

void loop() {   
  //faz a leitura do sensor de presença (retorna HIGH ou LOW)
  int sinal = digitalRead(PIN_SENSOR); 

  //HIGH : movimento detectado
  if(sinal == HIGH){
    //aciona o led
    Serial.print("high\n");
    digitalWrite(LED, HIGH); 
  }
  //LOW : nenhum movimento detectado
  else{
    //desativa o led
    Serial.print("low\n");
    digitalWrite(LED, LOW); 
  }
}
