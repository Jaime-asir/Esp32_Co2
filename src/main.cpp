#include <Arduino.h>
#include <PubSubClient.h> // Esta biblioteca instala las dependencias para usar el MQTT
#include <WiFi.h> // Esta biblioteca instala las dependencias para conectar mediante el wifi
#include <Adafruit_CCS811.h> // Esta biblioteca instala las dependencias para poder usar el sensor CCS811
#include <SPI.h> // El protocolo SPI se utiliza comúnmente para interconectar microcontroladores, sensores, pantallas y otros dispositivos periféricos en aplicaciones embebidas.
Adafruit_CCS811 miccs;
// Aqui configuraremosel entorno de ejecucion del programa antes de comenzar la funcion loop, la cual es el bucle principal del programa
void setup() {
 Serial.begin(9600);
 const char* ssid = "MiFibra-D0B7" ;
 const char* password = "opGLyZVV" ;
   WiFi.begin(ssid,password);
   while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.println("Connecting to WiFi..");

   }

  Serial.println("Connected to the WiFi network");



  Serial.println("miccs811 test");

  if(!miccs.begin()){
    Serial.println("Falla en el inicio del sensor! Porfavor revisa tu codigo.");
    while(1);
  }

  // Espera a que el sensor este disponible
  while(!miccs.available());
}

void loop() {

   if(miccs.available()){
    if(!miccs.readData()){ // Leer datos del CCS811
      Serial.print("CO2: ");
      Serial.print(miccs.geteCO2()); // Concentracion de dioxido de carbono
      Serial.print("ppm, TVOC: ");
      Serial.println(miccs.getTVOC()); // Compuestos Organicos Volatiles Totales
    }
    else{
      Serial.println("ERROR!");
      while(1);
    }
  }
  delay(500);
}