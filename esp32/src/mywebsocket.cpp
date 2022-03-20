#include <Arduino.h>
#include "mywebsocket.h"
// websocket
#include <ESPAsyncWebServer.h>
#include <vector>

// Create AsyncWebServer object on port 81
AsyncWebServer websocket_server(81);
AsyncWebSocket ws("/ws");
static int clientCounter = 0;

void mywebsocket::cleanupClients()
{
    // Serial.println("Cleanup Websockets");
    ws.cleanupClients(1);
    // delay(1000);
    // for (AsyncWebSocketClient *client : ws.getClients())
    // {
    //     Serial.printf("Cleanup Websockets: active client:%i -> Free Heap : %i\n", client->id(), ESP.getFreeHeap());
    // }
}
void mywebsocket::publishData(std::vector<float> data)
{

    if (clientCounter == 0)
    {
        return;
    }

    if (!ws.availableForWriteAll())
    {
        Serial.println("skip publishing data -> restart");
        // ws.cleanupClients();
        ESP.restart();
        return;
    }
    // Serial.printf("TMPL : Start -> Free Heap : %i\n", ESP.getFreeHeap());

    int decimals = 0;
    String result;
    result.reserve(3841);
    result.concat(String("["));
    for (int i = 0; i < data.size(); i++)
    {
        if (i == 0)
        {
            result.concat(String(data.at(i), decimals));
        }
        else
        {
            result.concat(String(","));
            result.concat(String(data.at(i), decimals));
        }
    }
    result.concat(String("]"));
    char char_array[result.length() + 1];

    result.toCharArray(char_array, result.length() + 1);

    for (AsyncWebSocketClient *client : ws.getClients())
    {
        ws.text(client->id(), char_array);
    }
    // Serial.printf("TMPL : DONE Sending Websocket -> Free Heap : %i\n", ESP.getFreeHeap());
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