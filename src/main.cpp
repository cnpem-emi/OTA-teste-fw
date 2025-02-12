#include <WiFi.h>
#include <HTTPClient.h>      //Biblioteca para realizar requisições por HTTP
#include <ESP32httpUpdate.h> //Biblioteca que possibilita OTA por checagem HTTP

// declaration for Wifi connection
#define WIFI_SSID "Devices"
#define WIFI_PASSWORD ""

//Variavel que armazena a versão do firmware indicada no Redis
uint8_t lastVersion = 0;

// Colocar a versão atual do firmware para a comparação
float versionCurrent = 1.0;

// Função de atualização do firmware via OTA
void updateOTA()
{
    // Se houver conexão WiFi
    if ((WiFi.status() == WL_CONNECTED))
    {

        // Realiza o download do firmware (.bin) e realiza a atualização
        t_httpUpdate_return ret = ESPhttpUpdate.update("sem link pois é o primeiro commit");

        // Switch para tratamento da resposta
        switch (ret)
        {
        case HTTP_UPDATE_FAILED: // Falha
            Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
            break;

        case HTTP_UPDATE_NO_UPDATES: // Ausência do arquivo de firmware
            Serial.println("HTTP_UPDATE_NO_UPDATES");
            break;

        case HTTP_UPDATE_OK: // Sucesso
            Serial.println("HTTP_UPDATE_OK");
            versionCurrent = lastVersion;
            break;
        }
    }
}

void checkUpdate()
{
    // Define timeout
    int timeout = 4000;

    if (versionCurrent != lastVersion) // verifica se as versões são diferentes
    {

        // Inicializa conexão por HTTP para acessar o firmware no servidor
        HTTPClient http;
        http.begin("sem link p/ primeiro commit");
        http.setConnectTimeout(timeout);
        http.setTimeout(timeout);
        int resCode = http.GET();
        Serial.println("Version: " + String(versionCurrent));
        if (resCode > 0)
        {
            // Se a versão for diferente da atual neste código, inicializa a atualização
            updateOTA();
        }
    }
}

void setup() {
  Serial.begin(9600);
  checkUpdate();
}

void loop() {}