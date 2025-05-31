#include <DHT.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <certificates.h>

#define trigger 27
#define echo 26
#define dhtpin 25
#define DHTTYPE DHT22
#define ledR 21
#define ledG 22
#define ledY 19

DHT dht(dhtpin, DHTTYPE);

// AWS
const char* endpoint = "avmr86pgqcbi1-ats.iot.us-east-2.amazonaws.com";
const char* awsTopic = "nivel_agua/entradas";

// configurando mqtt
const char* mqttServer = "broker.hivemq.com"; 
const int mqttPort = 1883;
const char* mqttTopic = "nivel_agua/entradas";

WiFiClient espClient; // para mqtt
PubSubClient mqttClient(espClient);

WiFiClientSecure net;  // para AWS
PubSubClient awsClient(net);

void conectarWifi(){
  WiFi.begin("Wokwi-GUEST", "", 6);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Wi-Fi conectado!");
}

// função para conectar ao MQTT
void connectarMQTT() {
  mqttClient.setServer(mqttServer, mqttPort);
  while (!mqttClient.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (mqttClient.connect("ESP32Client-1")) {
      Serial.println("Conectado ao MQTT!");
    } else {
      Serial.print("Falha na conexão.");
      Serial.print(mqttClient.state());
      delay(2000);
    }
  }
}

void conectarAWS(){
 // credenciais aws
  net.setCACert(aws_cert_ca);
  net.setCertificate(aws_cert_crt);
  net.setPrivateKey(aws_cert_private);

  awsClient.setServer(endpoint, 8883);
 
 
  Serial.println("Conectando ao AWS IOT");
 
  while (!awsClient.connected()) {
    Serial.print("...");
    if (awsClient.connect("ESP32ClientAWS")) {
      Serial.println("Conectado ao AWS IOT!");
    } else {
      delay(2000);
    }
  }
}

void setup() {
  
  Serial.begin(115200);
  pinMode(echo, INPUT);
  pinMode(trigger, OUTPUT);

  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledY, OUTPUT);

  conectarWifi();
  connectarMQTT();
  conectarAWS();
}

float nivelAgua() {
  digitalWrite(trigger, LOW); delayMicroseconds(2);
  digitalWrite(trigger, HIGH); delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  long duration = pulseIn(echo, HIGH);
  return duration * 0.034 / 2;
}

void loop() {
  if (!mqttClient.connected()) connectarMQTT();
  if (!awsClient.connected()) conectarAWS();

  if (Serial.available() > 0) {
    String comando = Serial.readStringUntil('\n');
    comando.trim();

    if (comando == "enviar") {
      float temperatura = dht.readTemperature();
      float umidade = dht.readHumidity();
      float nivel = nivelAgua();

      StaticJsonDocument<200> doc;
      doc["zona"] = "Zona 1";
      doc["temperatura"] = temperatura;
      doc["umidade"] = umidade;
      doc["nivel_agua"] = nivel;

      char jsonBuffer[256];
      serializeJson(doc, jsonBuffer);

      mqttClient.publish(mqttTopic, jsonBuffer);
      awsClient.publish(awsTopic, jsonBuffer);

      // 🟢 Console log simulando "console.log" do JavaScript
      Serial.println("Enviando dados:");
      Serial.print("JSON: ");
      Serial.println(jsonBuffer);

      // LED de risco
      if (nivel < 10) {
        digitalWrite(ledG, HIGH);
        digitalWrite(ledR, LOW);
        digitalWrite(ledY, LOW);
      } else if (nivel < 20) {
        digitalWrite(ledG, LOW);
        digitalWrite(ledR, LOW);
        digitalWrite(ledY, HIGH);
      } else {
        digitalWrite(ledG, LOW);
        digitalWrite(ledR, HIGH);
        digitalWrite(ledY, LOW);
      }
    } else {
      Serial.println("Comando não reconhecido. Digite 'enviar' para publicar os dados.");
    }
  }

}
