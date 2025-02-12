// #include <WiFi.h>
// #include <HTTPClient.h>      //Biblioteca para realizar requisições por HTTP
// #include <ESP32httpUpdate.h> //Biblioteca que possibilita OTA por checagem HTTP
// #include <Arduino.h>

// // declaration for Wifi connection
// #define WIFI_SSID "Devices"
// #define WIFI_PASSWORD NULL

// //Variaveis que armazenam a versão do firmware
// uint8_t lastVersion = 0;
// uint8_t versionCurrent = 1;

// void connectWiFi()
// {
//     WiFi.mode(WIFI_STA);
//     WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//     delay(500);
//     Serial.print("Conectando ao WiFi");

//     while (WiFi.status() != WL_CONNECTED)
//     {
//         Serial.print(".");
//         delay(500);

//     }

//     if (WiFi.status() == WL_CONNECTED)
//     {
//         Serial.println("\nWiFi conectado!");
//     }
//     else
//     {
//         Serial.println("\nFalha ao conectar ao WiFi.");
//     }
// }


// // Função de atualização do firmware via OTA
// void updateOTA()
// {   
//     Serial.println("Iniciando atualização OTA...");
//     // Realiza o download do firmware (.bin) e realiza a atualização
//     t_httpUpdate_return ret = ESPhttpUpdate.update("https://raw.githubusercontent.com/cnpem-emi/OTA-teste-fw/master/.pio/build/lolin_s2_mini/firmware.bin");

//     // Switch para tratamento da resposta
//     switch (ret)
//     {
//     case HTTP_UPDATE_FAILED: // Falha
//         Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
//         break;

//     case HTTP_UPDATE_NO_UPDATES: // Ausência do arquivo de firmware
//         Serial.println("HTTP_UPDATE_NO_UPDATES");
//         break;

//     case HTTP_UPDATE_OK: // Sucesso
//         Serial.println("HTTP_UPDATE_OK");
//         versionCurrent = lastVersion;
//         break;
//     }
// }

// void checkUpdate()
// {
//     Serial.println("Verificando atualização...");
//     // Define timeout
//     int timeout = 4000;

//     if (versionCurrent != lastVersion) // verifica se as versões são diferentes
//     {
//         Serial.println("Inicializando http");
//         // Inicializa conexão por HTTP para acessar o firmware no servidor
//         HTTPClient http;
//         http.begin("https://github.com/cnpem-emi/OTA-teste-fw/blob/master/.pio/build/lolin_s2_mini/firmware.bin");
//         http.setConnectTimeout(timeout);
//         http.setTimeout(timeout);
//         int resCode = http.GET();
//         Serial.println("Version: " + String(versionCurrent));
//         if (resCode > 0)
//         {
//             // Se a versão for diferente da atual neste código, inicializa a atualização
//             updateOTA();
//         }
//     }
// }

// void setup() {
//   Serial.begin(9600);
//   delay(2000);
//   connectWiFi();
//   checkUpdate();
// }

// void loop() {
// }

#include <Arduino.h>

void setup() {
 Serial.begin(9600);   
}

void loop() {
    Serial.println("NOVA VERSAO CARREGADA...");
}