// #include <WiFi.h>
// #include <HTTPClient.h>
// #include <ESP32httpUpdate.h>
// #include <Preferences.h>

// #define WIFI_SSID "Devices"
// #define WIFI_PASSWORD "" 

// void connectWiFi() {   
//     Serial.println("Iniciando conexão WiFi...");
//     WiFi.mode(WIFI_STA);
//     WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//     delay(500);

//     Serial.print("Conectando ao WiFi");
//     while (WiFi.status() != WL_CONNECTED) {
//         Serial.print(".");
//         delay(500);
//     }

//     if (WiFi.status() == WL_CONNECTED) {
//         Serial.println("\nWiFi conectado!");
//     } else {
//         Serial.println("\nFalha ao conectar ao WiFi.");
//     }
// }

// void updateOTA() {   

//     Serial.println("Iniciando atualização OTA...");

//     t_httpUpdate_return ret = ESPhttpUpdate.update("https://raw.githubusercontent.com/cnpem-emi/OTA-teste-fw/master/.pio/build/lolin_s2_mini/firmware.bin");

//     switch (ret) {
//     case HTTP_UPDATE_FAILED:
//         Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
//         break;

//     case HTTP_UPDATE_NO_UPDATES:
//         Serial.println("HTTP_UPDATE_NO_UPDATES");
//         break;

//     case HTTP_UPDATE_OK:
//         Serial.println("HTTP_UPDATE_OK");
//         break;
//     }
// }

// void checkUpdate() {

//         Serial.println("Inicializando HTTP para checagem de atualização...");
//             delay(500);
//             HTTPClient http;
//             http.begin("https://github.com/cnpem-emi/OTA-teste-fw/blob/master/.pio/build/lolin_s2_mini/firmware.bin");
//             http.setConnectTimeout(4000);
//             http.setTimeout(4000);
//             int resCode = http.GET();
//             if (resCode > 0) {  
//                 updateOTA();
//             }   
// }

// void setup() {
//     Serial.begin(9600);
//     delay(4000);
//     connectWiFi();
// }

// void loop() {
//     checkUpdate(); 
//     delay(3000);
// }

#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32httpUpdate.h>
#include <Preferences.h>

#define WIFI_SSID "Devices"
#define WIFI_PASSWORD "" 
#define FIRMWARE_VERSION 1  // Incrementar este número quando enviar nova versão ao GitHub

Preferences preferences;
bool updateChecked = false;  // Flag para garantir que só checa uma vez

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
        // Salva a nova versão na memória
        preferences.putInt("fwVersion", FIRMWARE_VERSION);
        break;
    }
}

void checkUpdate() {
    
    if (updateChecked) return;  // Se já checou uma vez, não checa novamente
    
    // Lê a versão atual salva na memória
    int currentVersion = preferences.getInt("fwVersion", 0);
    Serial.printf("Versão atual do firmware: %d\n", currentVersion);
    Serial.printf("Nova versão do firmware: %d\n", FIRMWARE_VERSION);

    // Só continua se a versão na memória for menor que a versão do código
    if (currentVersion < FIRMWARE_VERSION) {
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
    } else {
        Serial.println("Firmware já está atualizado!");
    }
    
    updateChecked = true;  // Marca que já checou
}

void setup() {
    Serial.begin(9600);
    delay(4000);
    preferences.begin("firmware", false);  // Inicia o namespace para as preferências
    connectWiFi();
}

void loop() {
    checkUpdate(); 
    delay(3000);
}