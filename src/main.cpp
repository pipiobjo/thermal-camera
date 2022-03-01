#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>             // File System for Web Server Files
#include <LittleFS.h>       // This file system is used.
#include "./buildinfiles.h" // include default responses
// #include <DNSServer.h>
#include <thermocam.h>
#include <Adafruit_MLX90640.h>
#include "./mywebsocket.h"

// camera
#include <Wire.h>

#define HOSTNAME "webserver"
// need a WebServer for http access on port 80.
ESP8266WebServer server(80);

// DNSServer dnsServer;
// const byte DNS_PORT = 53;

// websocket
mywebsocket websocket;

// camera
thermocam cam;
// const size_t pixelLength = 768;
// static float mlx90640To[pixelLength];

// ===== Simple functions used to answer simple GET requests =====
void handleRoot()
{
  String url = "/index.htm";

  if (!LittleFS.exists(url))
  {
    server.send(404, "text/html", FPSTR(notFoundContent));
  }

  server.sendHeader("Location", url, true);
  server.send(302);
}

void setup()
{
  Serial.begin(9600);

  Serial.println("Mounting the filesystem...\n");
  if (!LittleFS.begin())
  {
    Serial.println("could not mount the filesystem...\n");
    delay(200);
    ESP.restart();
  }

  Serial.println("");
  Serial.println("Starte WLAN-Hotspot \"astral\"");
  WiFi.mode(WIFI_AP);                // access point modus
  WiFi.softAP("astral", "12345678"); // Name des Wi-Fi netzes
  WiFi.begin();
  WiFi.setHostname(HOSTNAME);

  delay(500);                  // Abwarten 0,5s
  Serial.print("IP Adresse "); // Ausgabe aktueller IP des Servers
  Serial.println(WiFi.softAPIP());

  // register a redirect handler when only domain name is given.
  server.on("/", HTTP_GET, handleRoot);

  // serve all static files
  server.serveStatic("/", LittleFS, "/");

  server.onNotFound([]()
                    { server.send(404, "text/html", FPSTR(notFoundContent)); });

  server.begin();

  delay(500);
  String hostname = WiFi.getHostname();
  Serial.println(("hostname = " + hostname).c_str());

  // camera
  cam.setupCam();

  // websocket
  websocket.setupWebsocket();
}

void loop(void)
{
  server.handleClient();

  delay(600);
  websocket.cleanupClients();
  std::vector<float> v = cam.takeAPic();
  websocket.publishData(v);
}
