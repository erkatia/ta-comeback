#include <SPI.h>
#include <BH1750.h>
#include <LoRa.h>

BH1750 lightMeter;

#define ss 5
#define rst 14
#define dio0 15

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Receiver");

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  
  //replace the LoRa.begin(---E-) argument with your location's frequency 
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(915E6)) {
    Serial.println(".");
    delay(500);
  }
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
}

void loop() {
  // put your main code here, to run repeatedly:
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    
  // received a packet
  Serial.print("Received packet '");

  //Sensor Cahaya
  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  
  // read packet
    while (LoRa.available()) {
      String LoRaData = LoRa.readString();
      Serial.print(LoRaData); 
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
    
}
