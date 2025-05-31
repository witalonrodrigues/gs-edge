# Simulador de Monitoramento de Enchentes com IoT

Este projeto demonstra uma solução de monitoramento preventivo de enchentes utilizando sensores, microcontrolador ESP32, protocolo MQTT, e integração com a nuvem (AWS IoT). Também inclui um dashboard para visualização em tempo real.

## Problemática

Enchentes representam um grande risco às populações urbanas, especialmente em regiões com infraestrutura precária. A ausência de monitoramento em tempo real dificulta a ação preventiva.

## Solução Proposta

Um sistema de IoT que:

Captura dados de sensores (nível da água, temperatura, umidade)

Envia dados para um broker MQTT (HiveMQ) e também para AWS IoT

Aciona LEDs com base no nível de risco

Visualiza os dados em um dashboard Node-RED

Publica dados sob comando "enviar" digitado via Serial

## Componentes Utilizados

### Hardware (Camada IoT)

ESP32

Sensor ultrassônico HC-SR04 (nível da água)

Sensor DHT22 (temperatura e umidade)

LEDs RGB (indicadores de risco)

### Software (Back-end)

Protocolo MQTT (broker HiveMQ)

AWS IoT Core (com TLS e certificados)

PubSubClient (biblioteca MQTT para Arduino)

ArduinoJson

### Aplicativo (Dashboard)

Node-RED com dashboard

Conectado ao broker MQTT

## Fluxo de Dados

Sensores coletam os dados ambientais: nível da água, temperatura e umidade.

ESP32 processa os dados e publica no broker MQTT e AWS IoT.

Node-RED consome as mensagens do broker e exibe os dados no dashboard.

LEDs indicam o estado de risco:

Verde: seguro

Amarelo: alerta

Vermelho: perigo

## Instruções de Execução

Wokwi

Acesse o projeto: 
```
https://wokwi.com/projects/432344345072193537
```

Aguarde a conexão Wi-Fi

No console, digite: enviar

Observe o envio de dados para o console e LEDs mudando

Dashboard Node-RED

Configure Node-RED para consumir o tópico nivel_agua/entradas do HiveMQ

Exiba os dados usando gauges, textos ou indicadores visuais

##  Repositório

O repositório contém:

sketch.ino: Código-fonte do ESP32

diagram.json: Esquema de conexões do projeto

README.md: Este documento


## Autor

RM559023 - WITALON ANTONIO RODRIGUES
