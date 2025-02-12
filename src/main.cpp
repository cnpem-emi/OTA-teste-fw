#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32httpUpdate.h>
#include <Preferences.h>
#include <Arduino.h>

Preferences preferences;

// Configuração de WiFi
#define WIFI_SSID "Devices"
#define WIFI_PASSWORD ""  

// Variáveis para versão do firmware
uint8_t currentVersion = 2;  
uint8_t lastVersion = 1;

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

// Pega a versão do servidor
int getServerVersion() {
    HTTPClient http;
    http.begin("https://github.com/cnpem-emi/OTA-teste-fw/blob/master/.pio/build/lolin_s2_mini/firmware.bin");
    int httpCode = http.GET();
    
    if (httpCode > 0) {
        String version = http.getString();
        http.end();
        return version.toInt();
    }
    http.end();
    return -1;
}

// Atualização do firmware OTA
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
        preferences.putUInt("version", lastVersion); // Salva a nova versão
        ESP.restart(); // Reinicia após atualização bem-sucedida
        break;
    }
}

// Checagem de atualização
void checkUpdate() {
    Serial.println("Verificando atualização...");
    
    if (WiFi.status() != WL_CONNECTED) {
        connectWiFi();
        return;
    }

    int serverVersion = getServerVersion();
    if (serverVersion > currentVersion) {
        lastVersion = serverVersion;
        updateOTA();
    }
}

void setup() {
    Serial.begin(9600);
    delay(2000);
    
    preferences.begin("firmware", false);
    currentVersion = preferences.getUInt("version", 1); // Recupera versão salva
    
    connectWiFi();
}

void loop() {
    checkUpdate();  // Checa e faz update se necessário
    Serial.println("Versão: " + String(currentVersion));
    delay(5000);
}