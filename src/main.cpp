#include <Arduino.h>
#include <NTPClient.h>//Sincronizar la hora a través del Protocolo de Tiempo de Red (NTP, por sus siglas en inglés)
#include <WiFiUdp.h>//La biblioteca WiFiUdp es una biblioteca estándar del ESP32
#include <iostream> //Biblioteca usada para pasar una variable int a String
#include <string> //Biblioteca usada para pasar una variable int a String
#include <PubSubClient.h> // Esta biblioteca instala las dependencias para usar el MQTT
#include <WiFi.h> // Esta biblioteca instala las dependencias para conectar mediante el wifi
#include <Adafruit_CCS811.h> // Esta biblioteca instala las dependencias para poder usar el sensor CCS811
#include <SPI.h> // El protocolo SPI se utiliza comúnmente para interconectar microcontroladores, sensores, pantallas y otros dispositivos periféricos en aplicaciones embebidas.
#include <ArduinoJson.h>
#include <TimeLib.h>
Adafruit_CCS811 miccs;
// IMPORTANTE Aqui nos conectamos al punto de acceso
 const char* ssid = "tu_ssid" ;
 const char* password = "tu_contraseña" ;
// IMPORTANTE Información del servidor MQTT
const char* mqtt_server = "192.168.1.53";
const int mqtt_port = 1883;
//const char* mqtt_user = "user1@im.jaime.asir2.test";
//const char* mqtt_password = "user1";
// IMPORTANTE Nombre del cliente MQTT Y LOS TOPICS
const char* mqtt_client_name = "mi_esp32";
const char* mqtt_topic = "tema";
const char* mqtt_topic2 = "timestamp";
const char* mqtt_topic3 = "horario";
const char* jsonString = "jsonString";
// Instancia de WiFiClient y PubSubClient
WiFiClient espClient;
PubSubClient client(espClient);
// Configuración del cliente NTP
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
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

// IMPORTANTE Conexion al MQTT
    while (!client.connected()) {
    Serial.println("Conectando a MQTT...");
    if (client.connect(mqtt_client_name /*mqtt_user, mqtt_password*/)) {
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

    // establecer la fecha y hora local
  //setTime(17, 18, 0, 18, 4, 2023); //hora, minutos, segundos, dia, mes, año
  // ajustar la zona horaria
}

void loop() {
// Actualiza la hora del cliente NTP
  timeClient.update();
// Obtén el timestamp actual en segundos
  unsigned long timestamp = millis() / 1000;
// Crea un mensaje con el timestamp
  char timest[20];
  snprintf(timest, 20, "%lu", timestamp);
// Pasar a un formato adaptable para json y mqtt
  std::string str = std::to_string(miccs.geteCO2());
// Obtener la fecha y hora actual
  time_t t = now();
// Crear un objeto JSON con los datos
  StaticJsonDocument<200> jsonDocument;
  jsonDocument["VDP"] = timest; //VDP Son las siglas de Vida Del Programa, que hace referencia a cuantos segundos se lleva ejecutando
  jsonDocument["Co2"] = miccs.geteCO2();
  jsonDocument["Dia y hora"] = ctime(&t);
  
// Convertir el objeto JSON a una cadena de caracteres
  char jsonString[200];
  serializeJson(jsonDocument, jsonString);
// Publicar el mensaje en el topic "sensores"
char* pala=(char*)malloc(1000*sizeof(char));
strcpy(pala,"bedroom_temperature celsius=");

char* strnum=(char*)malloc(1000*sizeof(char));
//itoa(miccs.geteCO2(),strnum,100);
sprintf(strnum,"%d",miccs.geteCO2());


char* total=(char*)malloc(1000*sizeof(char));
strcpy(total,"");
strcat(total,pala);
strcat(total,strnum);
//strcat(pala,""+miccs.geteCO2());
Serial.print("-----------------------");
/*Serial.println(pala);
Serial.println(miccs.geteCO2());
Serial.println(strnum);*/
Serial.println(total);
client.publish("bedroom/temperature",total);

//client.publish("bedroom/temperature", "bedroom_temperature celsius=")&&str);
// Esta parte de aqui no tiene ningun efecto sobre el MQTT, son simples comprobaciones para saber si funciona correctamente el sensor el ESP32 y el codifgo
  if(miccs.available()){
    if(!miccs.readData()){ // Leer datos del CCS811
      //Serial.print(pala);
      /*Serial.print(" CO2: ");
      Serial.print(miccs.geteCO2()); // Concentracion de dioxido de carbono
      Serial.print(" ppm, TVOC: ");
      Serial.println(miccs.getTVOC()); // Compuestos Organicos Volatiles 
      Serial.print(" Momento de transimison ");
      Serial.print(timest);
      Serial.print(" Hora de transimison ");
      Serial.print(ctime(&t));*/
    }
    else{
      Serial.println("ERROR!");
      while(1);
    }
  }
  delay(20000);
}
