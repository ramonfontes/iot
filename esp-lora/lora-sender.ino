#include <SPI.h>
#include <LoRa.h>
#include "SSD1306.h"
#include<Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

//informações da rede WIFI
const char* ssid = "SSID";                 //SSID da rede WIFI
const char* password =  "PASSWORD";    //senha da rede wifi

//informações do broker MQTT - Verifique as informações geradas pelo CloudMQTT
const char* mqttServer = "SERVER_DOMAIN";   //server
const char* mqttUser = "USER";              //user
const char* mqttPassword = "PASSWORD";      //password
const int mqttPort = 1883;                     //port
const char* mqttTopicSub1 ="filhas";
 
SSD1306  display(0x3c, 4, 15);
 
#define SS      18
#define RST     14
#define DI0     26
#define BAND    915E6 //433E6 

WiFiClient espClient;
PubSubClient client(espClient);

 
void setup() {
  pinMode(25,OUTPUT); //Send success, LED will bright 1 second
  pinMode(16,OUTPUT);
  digitalWrite(16, LOW);    // set GPIO16 low to reset OLED
  delay(50); 
  digitalWrite(16, HIGH);
   
  Serial.begin(115200);
  while (!Serial); //If just the the basic function, must connect to a computer
  // Initialising the UI will init the display too.
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(5,5,"LoRa Filhas");
  display.display();
   
  SPI.begin(5,19,27,18);
  LoRa.setPins(SS,RST,DI0);
  Serial.println("LoRa Filhas");
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    #ifdef DEBUG
    Serial.println("Conectando ao WiFi..");
    #endif
  }
  
  #ifdef DEBUG
  Serial.println("Conectado na rede WiFi");
  #endif
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    #ifdef DEBUG
    Serial.println("Conectando ao Broker MQTT...");
    #endif
 
    if (client.connect("ESP8266Client1", mqttUser, mqttPassword )) {
      #ifdef DEBUG
      Serial.println("Conectado");  
      #endif
    } else {
      #ifdef DEBUG 
      Serial.print("falha estado  ");
      Serial.print(client.state());
      #endif
      delay(2000);
    }
  }

  //subscreve no tópico
  client.subscribe(mqttTopicSub1);
  Serial.println(WiFi.status());

  Serial.println("LoRa Initial OK!");
  display.drawString(5,20,"LoRa Initializing OK!");
  display.display();
  delay(2000);
}

void callback(char* topic, byte* payload, unsigned int length) {
  //armazena msg recebida em uma sring
  payload[length] = '\0';
  String strMSG = String((char*)payload);

  if (strcmp(topic,  mqttTopicSub1)==0){
      // send packet
      display.clear();
      display.setFont(ArialMT_Plain_16);
      display.drawString(3, 5, "Sending packet ");
      display.drawString(50, 30, strMSG);
      display.display();
      LoRa.beginPacket();
      LoRa.print(strMSG);
      LoRa.endPacket();
      digitalWrite(25, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(1000);                       // wait for a second
      digitalWrite(25, LOW);    // turn the LED off by making the voltage LOW
      delay(1000);                       // wait for a second
  } 
}

void loop() {
  client.loop();
}
