#include <SPI.h>
#include <LoRa.h>
#include "SSD1306.h"
 
SSD1306  display(0x3c, 4, 15);
 
#define SS      18
#define RST     14
#define DI0     26
#define BAND    915E6
#define L12 12
#define L13 13
#define L17 17
#define L21 21
#define L22 22
#define L23 23

void setup() {
  
  pinMode(16,OUTPUT);
  pinMode(L12, OUTPUT);
  pinMode(L13, OUTPUT);
  pinMode(L17, OUTPUT);
  pinMode(L21, OUTPUT);
  pinMode(L22, OUTPUT);
  pinMode(L23, OUTPUT);
  digitalWrite(16, LOW);    // set GPIO16 low to reset OLED
  delay(50); 
  digitalWrite(16, HIGH);
  
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
   
  Serial.begin(115200);
  while (!Serial); //if just the the basic function, must connect to a computer
  delay(1000);
  Serial.println("LoRa Receiver"); 
  display.drawString(5,5,"LoRa Receiver"); 
  display.display();
  SPI.begin(5,19,27,18);
  LoRa.setPins(SS,RST,DI0);
   
  if (!LoRa.begin(BAND)) {
    display.drawString(5,25,"Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initial OK!");
  display.drawString(5,25,"LoRa Initializing OK!");
  display.display();
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packets
    Serial.print("Received packet. ");
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.drawString(3, 0, "Received packet ");
    display.display();
    
    // read packet
    while (LoRa.available()) {
      String data = LoRa.readString();
      Serial.print(data);
      display.drawString(20,22, data);
      display.display();

      if (data == "22-on")
        digitalWrite(L22, HIGH);
      else if (data == "22-off")
        digitalWrite(L22, LOW);
      else if (data == "12-on")
        digitalWrite(L12, HIGH);
      else if (data == "12-off")
        digitalWrite(L12, LOW);
      else if (data == "13-on")
        digitalWrite(L13, HIGH);
      else if (data == "13-off")
        digitalWrite(L13, LOW);
      else if (data == "17-on")
        digitalWrite(L17, HIGH);
      else if (data == "17-off")
        digitalWrite(L17, LOW);
      else if (data == "21-on")
        digitalWrite(L21, HIGH);
      else if (data == "21-off")
        digitalWrite(L21, LOW);
      else if (data == "23-on")
        digitalWrite(L23, HIGH);
      else if (data == "23-off")
        digitalWrite(L23, LOW);
    }
    // print RSSI of packet
    Serial.print(" with RSSI ");
    Serial.println(LoRa.packetRssi());
    display.drawString(20, 45, "RSSI:  ");
    display.drawString(70, 45, (String)LoRa.packetRssi());
    display.display();
  }
}
