#include <Arduino.h>
#include "mywebsocket.h"
// websocket
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Create AsyncWebServer object on port 81
AsyncWebServer websocket_server(81);
AsyncWebSocket ws("/ws");
static int clientCounter = 0;

void mywebsocket::cleanupClients()
{
    ws.cleanupClients(4);
}
void mywebsocket::publishData(float data[], size_t size)
{

    if (clientCounter == 0)
    {
        return;
    }

    String result = "";
    for (int i = 0; i < size; i++)
    {
        if (i == 0)
        {
            result = result + data[i];
        }
        else
        {
            result = result + "," + data[i];
        }
    }

    ws.textAll(result);
}

static void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
                    void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
    case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        clientCounter++;
        break;
    case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        clientCounter--;
        break;
    case WS_EVT_DATA:
        // handleWebSocketMessage(arg, data, len);
        break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
        break;
    }
}

void mywebsocket::setupWebsocket()
{
    Serial.println("setup websocket");
    ws.onEvent(onEvent);
    websocket_server.addHandler(&ws);
    websocket_server.begin();
}