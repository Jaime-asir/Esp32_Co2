#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <Adafruit_CCS811.h>
#include <SPI.h>
Adafruit_CCS811 miccs;


void setup() {
 Serial.begin(9600);

  Serial.println("miccs811 test");

  if(!miccs.begin()){
    Serial.println("Falla en el inicio del sensor! Porfavor revisa tu codigo.");
    while(1);
  }

  // Espera a que el sensor este disponibre
  while(!miccs.available());
}

void loop() {

   if(miccs.available()){
    if(!miccs.readData()){ // Leer datos
      Serial.print("CO2: ");
      Serial.print(miccs.geteCO2());
      Serial.print("ppm, TVOC: ");
      Serial.println(miccs.getTVOC());
    }
    else{
      Serial.println("ERROR!");
      while(1);
    }
  }
  delay(500);
}