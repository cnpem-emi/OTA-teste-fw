#include "version.hpp"
#include <ArduinoJson.h>
#include "SPIFFS.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32httpUpdate.h>


#define WIFI_SSID "Devices"
#define WIFI_PASSWORD ""

Version version;

Version readVersionFile() {
    Version version = {0, 0};
    
    if(!SPIFFS.begin(true)) {
        Serial.println("Erro ao montar SPIFFS");
        return version;
    }
    
    File file = SPIFFS.open("/version.json", "r");
    if(!file) {
        Serial.println("Falha ao abrir arquivo de versão");
        return version;
    }
    
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, file);
    
    if(error) {
        Serial.println("Falha ao ler JSON");
        file.close();
        return version;
    }
    
    version.current = doc["currentVersion"] | 0;
    version.last = doc["lastVersion"] | 0;
    
    file.close();
    return version;
}

void updateLastVersion(int version) {
    if(!SPIFFS.begin(true)) {
        Serial.println("Erro ao montar SPIFFS");
        return;
    }
    
    File file = SPIFFS.open("/version.json", "r");
    if(!file) {
        Serial.println("Falha ao abrir arquivo de versão");
        return;
    }
    
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();
    
    if(error) {
        Serial.println("Falha ao ler JSON");
        return;
    }
    
    doc["lastVersion"] = version;
    
    file = SPIFFS.open("/version.json", "w");
    if(!file) {
        Serial.println("Falha ao abrir arquivo para escrita");
        return;
    }
    
    if(serializeJson(doc, file) == 0) {
        Serial.println("Falha ao escrever no arquivo");
    }
    
    file.close();
}


void connectWiFi() {   
    Serial.println("Iniciando conexão WiFi...");
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(500);
    
    Serial.print("Conectando ao WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi conectado!");
    } else {
        Serial.println("\nFalha ao conectar ao WiFi.");
    }
}

void updateOTA() {   
    Serial.println("Iniciando atualização OTA...");
    
    t_httpUpdate_return ret = ESPhttpUpdate.update("https://raw.githubusercontent.com/cnpem-emi/OTA-teste-fw/master/.pio/build/lolin_s2_mini/firmware.bin");

    switch (ret) {
    case HTTP_UPDATE_FAILED:
        Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
        break;

    case HTTP_UPDATE_NO_UPDATES:
        Serial.println("HTTP_UPDATE_NO_UPDATES");
        break;

    case HTTP_UPDATE_OK:
        Serial.println("HTTP_UPDATE_OK");
        updateLastVersion(version.current);
        Serial.print("Nova versão salva: ");
        Serial.println(version.current);
        break;
    }
}

void checkUpdate() {
    Serial.println("Verificando atualização...");
    
    if (version.current > version.last) {
        Serial.println("Inicializando HTTP para checagem de atualização...");
        delay(500);
        HTTPClient http;
        http.begin("https://github.com/cnpem-emi/OTA-teste-fw/blob/master/.pio/build/lolin_s2_mini/firmware.bin");
        http.setConnectTimeout(4000);
        http.setTimeout(4000);
        int resCode = http.GET();
        if (resCode > 0) {
            updateOTA();
        }
    }
}

void setup() {
    Serial.begin(9600);
    delay(2000);

    if(!SPIFFS.begin(true)) {
        Serial.println("Erro ao montar SPIFFS");
        return;
    }
    
    version = readVersionFile();
    Serial.printf("Versão atual: %d, Última versão: %d\n", version.current, version.last);
    
    connectWiFi();
}

void loop() {
    checkUpdate();
    delay(3000);
}