#include <SPI.h>
#include <BH1750.h>
#include <LoRa.h> //LORA
#include "RTClib.h" //RTC


BH1750 lightMeter;

#define ss 5
#define rst 14
#define dio0 2

int counter = 0;

void startLoRa(){
  
  Wire.begin();
  while (!Serial);
  Serial.println("LoRa Sender");
  //setup
  LoRa.setPins(ss, rst, dio0);

  //915E6 yang dipake versi NA
  while (!LoRa.begin(915E6)) {
    Serial.println(".");
    delay(500);
  }
  if (counter == 10) {
    // Increment readingID on every new reading
    Serial.println("Starting LoRa failed!"); 
  }
  
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
  }

void startBH(){
  lightMeter.begin();
  delay(2000);
  }
  

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  startLoRa();
  startBH();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  Serial.print("Sending packet: ");
  Serial.println(counter);

  //Send LoRa packet to receiver
  LoRa.beginPacket();
  LoRa.print("Light: ");
  float lux = lightMeter.readLightLevel();
  LoRa.print(lux);
  LoRa.println(" lx");
  delay(1000);
  LoRa.endPacket();

  counter++;

  delay(2000);
}
