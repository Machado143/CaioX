// main.cpp (versão com controle de relé via D0)
// Base: seu main.cpp original (temperatura + AP + LittleFS + /dados). :contentReference[oaicite:3]{index=3}

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <FS.h>
#include <LittleFS.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define ONE_WIRE_BUS D4  // sensor DS18B20 (mantido do original)
#define RELAY_PIN D0     // pino do relé que você quer controlar (D0 = GPIO16)

// Ajuste conforme seu relé: muitos módulos são ACTIVE LOW (0 = ligado).
// Se o seu módulo for ACTIVE HIGH, troque RELAY_ON_LEVEL para HIGH.
const int RELAY_ON_LEVEL = LOW;
const int RELAY_OFF_LEVEL = HIGH;

const char* ssid = "feira_ciencias";
const char* password = "asdf123456";

ESP8266WebServer server(80);

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
int n_sensores = 0;

bool relayState = false; // false = desligado, true = ligado

void setRelay(bool on) {
  relayState = on;
  pinMode(RELAY_PIN, on ? OUTPUT : INPUT);
  digitalWrite(RELAY_PIN, on ? RELAY_ON_LEVEL : RELAY_OFF_LEVEL);
}

void handleNotFound() {
  server.send(404, "text/plain", "Not found");
}

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(ONE_WIRE_BUS, INPUT_PULLUP);
  sensors.begin();

  // Inicializa pino do relé
  pinMode(RELAY_PIN, OUTPUT);
  // Garante estado inicial (desligado)
  setRelay(true);

  // Inicia Access Point (mantive seu modo AP original)
  WiFi.softAP(ssid, password, 1, false);
  Serial.println("Access Point iniciado!");
  Serial.print("IP do AP: ");
  Serial.println(WiFi.softAPIP());

  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed!");
    return;
  }

  // Serve arquivos estáticos (index.html e asti.css) se existirem em LittleFS
  server.on("/", HTTP_GET, []() {
    File file = LittleFS.open("/index.html", "r");
    if (!file) {
      server.send(404, "text/plain", "File not found");
      return;
    }
    server.streamFile(file, "text/html");
    file.close();
  });

  server.on("/esti.css", HTTP_GET, []() {
    File file = LittleFS.open("/esti.css", "r");
    if (!file) {
      server.send(404, "text/plain", "File not found");
      return;
    }
    server.streamFile(file, "text/css");
    file.close();
  });

  // Endpoint original de dados de temperatura
  server.on("/dados", HTTP_GET, []() {
    sensors.requestTemperatures();
    n_sensores = sensors.getDeviceCount();

    String json = "[";
    for (int i = 0; i < n_sensores; i++) {
      float temperatura = sensors.getTempCByIndex(i);
      DeviceAddress address;
      String endereco = "";
      if (sensors.getAddress(address, i)) {
        for (uint8_t j = 0; j < 8; j++) {
          endereco += String(address[j], HEX);
          if (j < 7) endereco += ":";
        }
      } else {
        endereco = "NA";
      }

      json += "{\"endereco\":\"" + endereco + "\",\"temperatura\":" + String(temperatura, 2) + "}";
      if (i < n_sensores - 1) json += ",";
    }
    json += "]";

    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", json);
  });

  // --- Endpoints do relé ---
  server.on("/relay/on", HTTP_GET, []() {
    setRelay(true);
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", "{\"relay\":\"on\"}");
  });

  server.on("/relay/off", HTTP_GET, []() {
    setRelay(false);
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", "{\"relay\":\"off\"}");
  });

  server.on("/relay/toggle", HTTP_GET, []() {
    setRelay(relayState == false);
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", String("{\"relay\":\"") + (relayState ? "on" : "off") + "\"}");
  });

  server.on("/relay/status", HTTP_GET, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", String("{\"relay\":\"") + (relayState ? "on" : "off") + "\"}");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
