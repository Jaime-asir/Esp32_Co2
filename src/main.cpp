#include <Arduino.h>
#include <iostream> //Biblioteca usada para pasar una variable int a String
#include <string> //Biblioteca usada para pasar una variable int a String
#include <PubSubClient.h> // Esta biblioteca instala las dependencias para usar el MQTT
#include <WiFi.h> // Esta biblioteca instala las dependencias para conectar mediante el wifi
#include <Adafruit_CCS811.h> // Esta biblioteca instala las dependencias para poder usar el sensor CCS811
#include <SPI.h> // El protocolo SPI se utiliza comúnmente para interconectar microcontroladores, sensores, pantallas y otros dispositivos periféricos en aplicaciones embebidas.
Adafruit_CCS811 miccs;

 const char* ssid = "MiFibra-D0B7" ;
 const char* password = "opGLyZVV" ;
 // Información del servidor MQTT
const char* mqtt_server = "192.168.1.103";
const int mqtt_port = 1883;
const char* mqtt_user = "user1@im.jaime.asir2.test";
const char* mqtt_password = "user1";
// Nombre del cliente MQTT y de los temas
const char* mqtt_client_name = "mi_esp32";
const char* mqtt_topic = "tema";
// Instancia de WiFiClient y PubSubClient
WiFiClient espClient;
PubSubClient client(espClient);


// Aqui configuraremos el entorno de ejecucion del programa antes de comenzar la funcion loop, la cual es el bucle principal del programa
void setup() {
 Serial.begin(9600);


   WiFi.begin(ssid,password);
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
   }
    Serial.println("Connected to the WiFi network");
 // Configuración de la conexión MQTT
  client.setServer(mqtt_server, mqtt_port);


    while (!client.connected()) {
    Serial.println("Conectando a MQTT...");
    if (client.connect(mqtt_client_name, mqtt_user, mqtt_password)) {
      Serial.println("Conexión exitosa.");
    } else {
      Serial.print("Error de conexión: ");
      Serial.println(client.state());
      delay(2000);
    }
  }





  Serial.println("miccs811 test");

  if(!miccs.begin()){
    Serial.println("Falla en el inicio del sensor! Porfavor revisa tu codigo.");
    while(1);
  }

  // Espera a que el sensor este disponible
  while(!miccs.available());
}

void loop() {
  std::string str = std::to_string(miccs.geteCO2());
  if (client.connected()) {
    client.publish(mqtt_topic, str.c_str());
    delay(500);
  } 
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
