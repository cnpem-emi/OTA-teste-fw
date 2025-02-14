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

#define FIRMWARE_VERSION 1 // Atualize este número sempre que recompilar o código

Preferences preferences;

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
        Serial.printf("Erro na atualização (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
        break;

    case HTTP_UPDATE_NO_UPDATES:
        Serial.println("Nenhuma atualização disponível.");
        break;

    case HTTP_UPDATE_OK:
        Serial.println("Atualização OTA concluída com sucesso!");
        // 🔹 Salva a nova versão na memória flash
        preferences.putInt("fwVersion", FIRMWARE_VERSION);
        preferences.end();
        break;
    }
}

void checkUpdate() { 
    // Lê a versão salva na memória flash
    int storedVersion = preferences.getInt("fwVersion", 0);
    Serial.printf("Versão atual salva na flash: %d\n", storedVersion);
    Serial.printf("Versão do firmware no código: %d\n", FIRMWARE_VERSION);

    if (storedVersion < FIRMWARE_VERSION) {
        Serial.println("Nova versão detectada! Iniciando atualização...");
        updateOTA();
    } else {
        Serial.println("Firmware já está atualizado!");
    }
}

void setup() {
    Serial.begin(9600);
    delay(4000);
    preferences.begin("firmware", false);  // Inicia o namespace de armazenamento
    connectWiFi();
}

void loop() {
    checkUpdate(); 
    Serial.println("====== INFORMAÇÃO DO FIRMWARE ======");
    Serial.printf("Versão do firmware em execução: %d\n", preferences.getInt("fwVersion", 0));
    Serial.println("==================================");
    
    delay(5000);
}
