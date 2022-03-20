#include "mywebserver.h"
#include "ESPAsyncWebServer.h"
#include "Arduino.h"
#include "SPIFFS.h"

AsyncWebServer server(80);

void mywebserver::setupWebserver()
{

    // Initialize LittleFS
    if (!SPIFFS.begin(false /* false: Do not format if mount failed */))
    {
        Serial.println("Failed to mount LittleFS");
        if (!SPIFFS.begin(true /* true: format */))
        {
            Serial.println("Failed: could not mount the filesystem...");
            delay(200);
            ESP.restart();
        }
        else
        {
            Serial.println("LittleFS formatted successfully");
        }
    }

    unsigned int totalBytes = SPIFFS.totalBytes();
    unsigned int usedBytes = SPIFFS.usedBytes();

    Serial.println("File sistem info.");

    Serial.print("Total space:      ");
    Serial.print(totalBytes);
    Serial.println("byte");

    Serial.print("Total space used: ");
    Serial.print(usedBytes);
    Serial.println("byte");

    Serial.print("Total space free: ");
    Serial.print(totalBytes - usedBytes);
    Serial.println("byte");

    Serial.println();

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/index.html", "text/html"); });
    server.begin();
}
