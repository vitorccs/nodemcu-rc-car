#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include <JoyCoords.h>

typedef std::function<void(JoyCoords coords)> CoordsHandlerFunction;
typedef std::function<void(bool buttonToggle)> ButtonAToggleHandlerFunction;

class SocketServer
{
public:
    SocketServer();

    void init(CoordsHandlerFunction coordsHandler,
              ButtonAToggleHandlerFunction buttonAHandler);

    void handleWebSocketMessage(void *arg,
                                uint8_t *data,
                                size_t length);

    void onEvent(AsyncWebSocket *webSocket,
                 AsyncWebSocketClient *client,
                 AwsEventType type,
                 void *arg,
                 uint8_t *data,
                 size_t length);

private:
    AsyncWebServer webServer;
    AsyncWebSocket webSocket;
    CoordsHandlerFunction _coordsHandler;
    ButtonAToggleHandlerFunction _buttonAHandler;
    const char *ALIAS_DIRECTION;
    const char *ALIAS_SPEED;
    const char *ALIAS_BUTTON_A;
};

#endif