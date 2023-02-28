#include <SPI.h>
#include <BH1750.h>
#include <LoRa.h>

BH1750 lightMeter;

#define ss 5
#define rst 14
#define dio0 2

int counter = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  lightMeter.begin(); //sensor cahaya
  while (!Serial);
  Serial.println("LoRa Sender");
  //setup
  LoRa.setPins(ss, rst, dio0);

  //915E6 yang dipake versi NA
  while (!LoRa.begin(915E6)) {
    Serial.println(".");
    delay(500);
  }
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Sending packet: ");
  Serial.println(counter);

  //Send LoRa packet to receiver
  LoRa.beginPacket();
  LoRa.print("hello ");
  
  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  
  LoRa.print(counter);
  delay(1000);
  LoRa.endPacket();

  counter++;

  delay(500);
}
