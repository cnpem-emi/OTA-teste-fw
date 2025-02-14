// #include <WiFi.h>
// #include <HTTPClient.h>
// #include <ESP32httpUpdate.h>
// #include <Preferences.h>

// Preferences preferences;
// int contador = 0;

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
//     // Inicia as preferências
//     preferences.begin("minhaApp", false);
//     // Lê o valor atual do contador
//     contador = preferences.getInt("contador", 0);
//     // Incrementa o contador
//     contador++;
//     // Salva o novo valor
//     preferences.putInt("contador", contador);
// }

// void loop() {
//     checkUpdate(); 
//     delay(3000);
// }

#include <Preferences.h>

Preferences preferences;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  preferences.begin("minhaApp", true);  // true = somente leitura
}

void loop() {
  // Lê o valor do contador
  int contador = preferences.getInt("contador", 0);
  
  Serial.println("====== TESTE DE PERSISTÊNCIA PÓS-OTA ======");
  Serial.print("Valor atual do contador na memória: ");
  Serial.println(contador);
  Serial.println("=========================================");
  
  delay(1000);  // Espera 1 segundo antes de mostrar novamente
}
