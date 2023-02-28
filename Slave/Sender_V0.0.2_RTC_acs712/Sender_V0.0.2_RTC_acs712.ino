#include <SPI.h>
#include <BH1750.h>
#include <LoRa.h> //LORA
#include "RTClib.h" //RTC

//RTC
RTC_DS1307 DS1307_RTC;
char Week_days[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//BH1750
BH1750 lightMeter;

//ACS712
int OUT = 34;

const int Num_readings = 500;
long int sensor_value = 0;  

float voltage = 0;
float current = 0;

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

void startRTC(){
    //RTC
  #ifndef ESP32
  while (!Serial); // wait for serial port to connect. Needed for native USB
  #endif
  if (!DS1307_RTC.begin()) {
    Serial.println("Couldn't find RTC");
    while(1);
  }
   DS1307_RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //RTC
  }

void startRTC1(){
    DateTime now = DS1307_RTC.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(Week_days[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

    Serial.print(" since midnight 1/1/1970 = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");

    // calculate a date which is 7 days, 12 hours, 30 minutes, 6 seconds into the future
    DateTime future (now + TimeSpan(7,12,30,6));

    Serial.print(" now + 7d + 12h + 30m + 6s: ");
    Serial.print(future.year(), DEC);
    Serial.print('/');
    Serial.print(future.month(), DEC);
    Serial.print('/');
    Serial.print(future.day(), DEC);
    Serial.print(' ');
    Serial.print(future.hour(), DEC);
    Serial.print(':');
    Serial.print(future.minute(), DEC);
    Serial.print(':');
    Serial.print(future.second(), DEC);

    Serial.println();
    Serial.println();
    delay(3000);
  }

void ACS(){
  for (int i = 0; i < Num_readings; i++)
  {
    sensor_value += analogRead(OUT);
    delay(2);
  }

  sensor_value = sensor_value / Num_readings;
  voltage = sensor_value * 5.0 / 1024.0;
  current = (voltage - 2.5) / 0.185;
  }

void ACSLoop(){
  Serial.print("ADC Value: ");
  Serial.print(sensor_value);
  
  Serial.print("  ADC Voltage: ");
  Serial.print(voltage);
  Serial.print("V");
  
  Serial.print("  Current: ");
  Serial.print(current);
  Serial.println("A");
  
  }

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  startLoRa();
  startBH();
  startRTC();
  ACS();
}

void loop() {
  // put your main code here, to run repeatedly:
  startRTC1();
  ACSLoop();
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
