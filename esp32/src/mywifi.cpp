#include <Arduino.h>
#include <WiFi.h>
#include "mywifi.h"
#define HOSTNAME "webserver"

void mywifi::setupWifi()
{
    Serial.println("");
    Serial.println("Starte WLAN-Hotspot \"thermalcam\"");
    WiFi.mode(WIFI_AP);                    // access point modus
    WiFi.softAP("thermalcam", "12345678"); // Name des Wi-Fi netzes
    WiFi.begin();
    // WiFi.setHostname(HOSTNAME);

    delay(500);                  // Abwarten 0,5s
    Serial.print("IP Adresse "); // Ausgabe aktueller IP des Servers
    Serial.println(WiFi.softAPIP());
}