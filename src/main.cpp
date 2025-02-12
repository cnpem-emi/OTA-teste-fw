#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32httpUpdate.h>

#define WIFI_SSID "Devices"
#define WIFI_PASSWORD ""  

// Variáveis para versão do firmware
int currentVersion = 2;
int lastVersion = 1;


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
        currentVersion = lastVersion + 1;
        break;
    }
}

// Checagem de atualização
void checkUpdate() {
    
    Serial.println("Verificando atualização...");
    
    if (currentVersion != lastVersion) {

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
    connectWiFi();
}

void loop() {
    checkUpdate();
    delay(3000);
}
