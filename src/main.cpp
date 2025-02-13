#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32httpUpdate.h>
#include <Preferences.h>

#define WIFI_SSID "Devices"
#define WIFI_PASSWORD ""

Preferences preferences;
const String firmwareVersion = "1.0.1";  // Alterar manualmente quando for uma nova versão

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
            preferences.putString("fw_version", firmwareVersion);  // Salva a nova versão na memória flash
            break;
    }
}

void checkUpdate() {
    Serial.println("Verificando versão do firmware...");

    preferences.begin("ota", false);
    String storedVersion = preferences.getString("fw_version", "0.0.0");  // Versão salva ou "0.0.0" se for a primeira vez

    Serial.print("Versão armazenada: ");
    Serial.println(storedVersion);
    Serial.print("Versão atual no código: ");
    Serial.println(firmwareVersion);



    if (storedVersion != firmwareVersion) {
        Serial.println("Nova versão detectada! Iniciando atualização...");
        HTTPClient http;
        http.begin("https://github.com/cnpem-emi/OTA-teste-fw/blob/master/.pio/build/lolin_s2_mini/firmware.bin");
        http.setConnectTimeout(4000);
        http.setTimeout(4000);
        int resCode = http.GET();
        if (resCode > 0) {  
            updateOTA();
        }   
    } else {
        Serial.println("Nenhuma atualização necessária.");
    }

    preferences.end();
}

void setup() {
    Serial.begin(9600);
    delay(5000);
    connectWiFi();
}

void loop() {
    checkUpdate();
    delay(4000); 
}


//---------------------------------------

// #include <WiFi.h>
// #include <HTTPClient.h>
// #include <ESP32httpUpdate.h>

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
//     Serial.println("Version: ");
//     delay(3000);
// }

// #include <Arduino.h> 
// void setup() {Serial.begin(9600);} void loop() {Serial.println("firmware que está compilado e nao foi carregado via serial, apenas versionado para o git");}
