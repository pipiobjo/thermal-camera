#include <Arduino.h>
#include <Wire.h>

// #include "MLX90640_API.h"
// #include "MLX90640_I2C_Driver.h"
#define LED 2
// wifi
#include "mywifi.h"
mywifi _wifi;

// webserver
#include "mywebserver.h"
mywebserver _mywebserver;

// websocket
#include "./mywebsocket.h"
mywebsocket _websocket;

// camera
#include <thermocam.h>
thermocam _cam;
// const byte MLX90640_address = 0x33; // Default 7-bit unshifted address of the MLX90640
// #define TA_SHIFT 8                  // Default shift for MLX90640 in open air
// static float mlx90640To[768];
// paramsMLX90640 mlx90640;

// // Returns true if the MLX90640 is detected on the I2C bus
// boolean isConnected()
// {
//     // Connect thermal sensor.
//     Wire.begin();
//     Wire.setClock(400000); // Increase I2C clock speed to 400kHz
//     Wire.beginTransmission((uint8_t)MLX90640_address);
//     if (Wire.endTransmission() != 0)
//     {
//         Serial.println("MLX90640 not detected at default I2C address. Please check wiring.");
//         return false;
//     }
//     else
//     {
//         Serial.println("MLX90640 online!");
//         return true;
//     }
// }

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);

    _wifi.setupWifi();
    _mywebserver.setupWebserver();

    // camera
    _cam.setupCam();

    // websocket
    _websocket.setupWebsocket();

    pinMode(LED, OUTPUT);
}

void loop()
{
    // // put your main code here, to run repeatedly:
    // digitalWrite(LED, HIGH);
    // // Serial.println("LED is on");
    // delay(1000);
    // digitalWrite(LED, LOW);
    // // Serial.println("LED is off");
    delay(600);

    _websocket.cleanupClients();
    std::vector<float> v = _cam.takeAPic();
    _websocket.publishData(v);
    return;
}
