#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SocketServer.h>
#include <iostream>
#include <stdexcept>
#include <JoyCoords.h>

const char indexHtml[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no" />
    <title>JoyStick</title>
    <style>
        /* global */
        *,
        ::after,
        ::before {
            box-sizing: content-box;
        }

        html,
        body {
            padding: 0;
            margin: 0;
        }

        body {
            width: 100%;
            height: 100%;
            overflow: hidden;
            font-family: 'Roboto', 'Tahoma', 'Arial', sans-serif;
            background: #6c8bee;
        }

        button {
            background: radial-gradient(ellipse at center, #444444 0%, #222222 100%);
            border: 1px solid #222;
            color: #fff;
            padding: 1rem clamp(1rem, 6vh, 3rem);
            font-size: clamp(1rem, 2.5vw, 2rem);
            border-radius: .4rem;
            margin: 1rem 0;
            user-select: none;
        }

        button.pressed {
            background: #444;
            box-shadow: inset 3px 1px 7px 4px #222;
        }


        /* debug */
        .debug {
            position: absolute;
            background-color: rgba(0, 0, 0, .6);
            color: #ccc;
            bottom: 0;
            right: 0;
            margin: 0 auto;
            padding: .4rem .5rem;
        }

        /* container */
        .container {
            display: flex;
            align-items: center;
            width: 100%;
            height: 100%;
        }

        /* container elements */
        .container .joystick {
            flex-grow: 2;
            display: flex;
            justify-content: center;
            width: 100%;
            border: 0px solid red;
        }

        .container .buttons {
            flex-grow: 1;
            display: flex;
            flex-direction: column;
            justify-content: end;
            padding: 0 5rem;
            border: 0px solid green;
        }

        .container .joystick #joystick-canvas {
            width: 73%;
            aspect-ratio: 1/1;
        }


        /* buttons container */
        .buttons button.pressed:nth-child(1) {
            color: yellow;
        }

        @media (max-width: 550px) {
            .container {
                flex-direction: column;
            }
        }
    </style>
    <script>
        /*
        * Name          : joy.js
        * @author       : Roberto D'Amico (Bobboteck)
        * Last modified : 09.06.2020
        * Revision      : 1.1.6
        * Source        : https://github.com/bobboteck/JoyStick
        * License       : The MIT License (MIT)
        */
        let StickStatus = { xPosition: 0, yPosition: 0, x: 0, y: 0, cardinalDirection: "C" }; var JoyStick = function (t, e, i) { var o = void 0 === (e = e || {}).title ? "joystick" : e.title, n = void 0 === e.width ? 0 : e.width, a = void 0 === e.height ? 0 : e.height, r = void 0 === e.internalFillColor ? "#00AA00" : e.internalFillColor, s = void 0 === e.internalLineWidth ? 2 : e.internalLineWidth, c = void 0 === e.internalStrokeColor ? "#003300" : e.internalStrokeColor, $ = void 0 === e.externalLineWidth ? 2 : e.externalLineWidth, d = void 0 === e.externalStrokeColor ? "#008000" : e.externalStrokeColor, u = void 0 === e.autoReturnToCenter || e.autoReturnToCenter; i = i || function (t) { }; var h = document.getElementById(t); h.style.touchAction = "none"; var S = document.createElement("canvas"); S.id = o, 0 === n && (n = h.clientWidth), 0 === a && (a = h.clientHeight), S.width = n, S.height = a, h.appendChild(S); var f = S.getContext("2d"), l = 0, k = 2 * Math.PI, _ = (S.width - (S.width / 2 + 10)) / 2, g = _ + 5, x = _ + .6 * _, v = S.width / 2, P = S.height / 2, C = S.width / 10, p = -1 * C, y = S.height / 10, w = -1 * y, L = v, F = P; function m() { f.beginPath(), f.arc(v, P, x, 0, k, !1), f.lineWidth = $, f.strokeStyle = d, f.stroke() } function E() { f.beginPath(), L < _ && (L = g), L + _ > S.width && (L = S.width - g), F < _ && (F = g), F + _ > S.height && (F = S.height - g), f.arc(L, F, _, 0, k, !1); var t = f.createRadialGradient(v, P, 5, v, P, 200); t.addColorStop(0, r), t.addColorStop(1, c), f.fillStyle = t, f.fill(), f.lineWidth = s, f.strokeStyle = c, f.stroke() } function W(t) { l = 1 } function T(t) { l = 1 } function D() { let t = "", e = L - v, i = F - P; return i >= w && i <= y && (t = "C"), i < w && (t = "N"), i > y && (t = "S"), e < p && ("C" === t ? t = "W" : t += "W"), e > C && ("C" === t ? t = "E" : t += "E"), t } "ontouchstart" in document.documentElement ? (S.addEventListener("touchstart", W, !1), document.addEventListener("touchmove", function t(e) { 1 === l && e.targetTouches[0].target === S && (L = e.targetTouches[0].pageX, F = e.targetTouches[0].pageY, "BODY" === S.offsetParent.tagName.toUpperCase() ? (L -= S.offsetLeft, F -= S.offsetTop) : (L -= S.offsetParent.offsetLeft, F -= S.offsetParent.offsetTop), f.clearRect(0, 0, S.width, S.height), m(), E(), StickStatus.xPosition = L, StickStatus.yPosition = F, StickStatus.x = (100 * ((L - v) / g)).toFixed(), StickStatus.y = (-(100 * ((F - P) / g) * 1)).toFixed(), StickStatus.cardinalDirection = D(), i(StickStatus)) }, !1), document.addEventListener("touchend", function t(e) { l = 0, u && (L = v, F = P), f.clearRect(0, 0, S.width, S.height), m(), E(), StickStatus.xPosition = L, StickStatus.yPosition = F, StickStatus.x = (100 * ((L - v) / g)).toFixed(), StickStatus.y = (-(100 * ((F - P) / g) * 1)).toFixed(), StickStatus.cardinalDirection = D(), i(StickStatus) }, !1)) : (S.addEventListener("mousedown", T, !1), document.addEventListener("mousemove", function t(e) { 1 === l && (L = e.pageX, F = e.pageY, "BODY" === S.offsetParent.tagName.toUpperCase() ? (L -= S.offsetLeft, F -= S.offsetTop) : (L -= S.offsetParent.offsetLeft, F -= S.offsetParent.offsetTop), f.clearRect(0, 0, S.width, S.height), m(), E(), StickStatus.xPosition = L, StickStatus.yPosition = F, StickStatus.x = (100 * ((L - v) / g)).toFixed(), StickStatus.y = (-(100 * ((F - P) / g) * 1)).toFixed(), StickStatus.cardinalDirection = D(), i(StickStatus)) }, !1), document.addEventListener("mouseup", function t(e) { l = 0, u && (L = v, F = P), f.clearRect(0, 0, S.width, S.height), m(), E(), StickStatus.xPosition = L, StickStatus.yPosition = F, StickStatus.x = (100 * ((L - v) / g)).toFixed(), StickStatus.y = (-(100 * ((F - P) / g) * 1)).toFixed(), StickStatus.cardinalDirection = D(), i(StickStatus) }, !1)), m(), E(), this.GetWidth = function () { return S.width }, this.GetHeight = function () { return S.height }, this.GetPosX = function () { return L }, this.GetPosY = function () { return F }, this.GetX = function () { return (100 * ((L - v) / g)).toFixed() }, this.GetY = function () { return (-(100 * ((F - P) / g) * 1)).toFixed() }, this.GetDir = function () { return D() } };
    </script>
    <script>
        class SocketClient {
            constructor() {
                this.connection = new WebSocket(`ws://${window.location.hostname}/ws`);
                this.connection.onopen = () => this.onOpen();
                this.connection.onerror = (error) => this.onError(error);
            }

            send(params) {
                const jsonString = JSON.stringify(params);
                console.log(jsonString);
                document.querySelector('.debug').innerHTML = jsonString;
                this.connection.send(jsonString);
            }

            setOnMessage(handler) {
                this.connection.onmessage = (e) => handler(e);
            }

            onOpen() {
                console.log('WebSocket Connection at ' + window.location.hostname);
            }

            onError(error) {
                console.log('WebSocket Error ' + error);
                alert('WebSocket Error #' + error);
            }
        }

        class MessageHandler {
            constructor(socketClient) {
                this.socketClient = socketClient
                this.socketClient.setOnMessage(this.handle);
            }

            handle(e) {
                console.log('Message received: ' + e.data);
            }
        }

        class ToggleButton {
            constructor(selector,
                        callback,
                        initivalValue = false) {
                this.element = document.querySelector(selector);
                this.callback = callback;
                this.setValue(initivalValue);
                this.setEvent();
            }

            setEvent() {
                this.element.addEventListener('click', () => {
                    this.toggle()
                    this.callback(this.value);
                });
            }

            toggle() {
                this.setValue(!this.value);
            }

            setValue(value) {
                this.value = value;
                this.element.classList.toggle('pressed', value);
            }
        }

        class PushButton {
            constructor(selector,
                        callback,
                        initivalValue = false) {
                this.element = document.querySelector(selector);
                this.callback = callback;
                this.setValue(initivalValue);
                this.timer = null;
                this.setEvent();
            }

            setEvent() {
                this.element.addEventListener('pointerdown', () => {
                    this.setValue(true);
                    this.callback(this.value);
                    this.timer = window.setInterval(() => this.callback(this.value), 290);
                });

                this.element.addEventListener('pointerup', () => {
                    window.clearInterval(this.timer);
                    this.setValue(false);
                    this.callback(this.value);
                });
            }


            setValue(value) {
                this.value = value;
                this.element.classList.toggle('pressed', value);
            }
        }

        class MessageSender {
            constructor(socketClient, 
                        joystick, 
                        buttonASel, 
                        buttonBSel,
                        buttonCSel) {
                this.socketClient = socketClient;
                this.joystick = joystick;
                this.buttonA = new ToggleButton(buttonASel, (value) => this.sendButtonA(value));

                window.setInterval(() => this.sendJoystick(), 50)
            }

            sendButtonA(value) {
                const params = { 'button-a': value };

                this.socketClient.send(params);
            }

            sendJoystick() {
                const xAxis = this.joystick.GetX();
                const yAxis = this.joystick.GetY();
                const direction = this.joystick.GetDir();
                const speed = Math.abs(xAxis) > Math.abs(yAxis)
                    ? Math.abs(xAxis)
                    : Math.abs(yAxis);
                const params = { direction, speed };

                this.socketClient.send(params);
            }
        }

        window.addEventListener('load', () => {
            const socketClient = new SocketClient();
            const messageHandler = new MessageHandler(socketClient);
            const joystick = new JoyStick('joystick-canvas', {
                'internalFillColor': '#444',
                'internalStrokeColor': '#222',
                'externalStrokeColor': '#222'
            });
            const messageSender = new MessageSender(
                socketClient,
                joystick,
                '#button-a'
            );
        });
    </script>


</head>

<body>
    <section class="debug"></section>

    <section class="container">
        <div class="joystick">
            <div id="joystick-canvas"></div>
        </div>
        <div class="buttons">
            <button type="button" id="button-a">Lights</button>
        </div>
    </section>
</body>

</html>
)rawliteral";

SocketServer::SocketServer() : webServer(80),
                               webSocket("/ws")
{

    ALIAS_BUTTON_A = "button-a";
    ALIAS_DIRECTION = "direction";
    ALIAS_SPEED = "speed";

    AwsEventHandler handler = [&](AsyncWebSocket *server,
                                  AsyncWebSocketClient *client,
                                  AwsEventType type,
                                  void *arg,
                                  uint8_t *data,
                                  size_t length)
    {
        return onEvent(server, client, type, arg, data, length);
    };

    webSocket.onEvent(handler);
    webServer.addHandler(&webSocket);

    ArRequestHandlerFunction onRequest = [](AsyncWebServerRequest *request)
    {
        request->send_P(200, "text/html", indexHtml);
    };

    webServer.on("/", HTTP_GET, onRequest);
}

void SocketServer::init(CoordsHandlerFunction coordsHandler,
                        ButtonAToggleHandlerFunction buttonAHandler)
{
    _coordsHandler = coordsHandler;
    _buttonAHandler = buttonAHandler;

    webServer.begin();
}

void SocketServer::onEvent(AsyncWebSocket *server,
                           AsyncWebSocketClient *client,
                           AwsEventType type,
                           void *arg,
                           uint8_t *data,
                           size_t length)
{
    switch (type)
    {
    case WS_EVT_CONNECT:
    {
        // prevent multiple client websocket connections
        if (server->count() == 1)
        {
            Serial.printf("Client WebSocket #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        }
        else
        {
            Serial.printf("Client WebSocket #%u from %s was rejected\n", client->id(), client->remoteIP().toString().c_str());
            server->close(client->id());
        }
        break;
    }
    case WS_EVT_DISCONNECT:
    {
        Serial.printf("Client WebSocket #%u disconnected\n", client->id());
        break;
    }
    case WS_EVT_DATA:
    {
        handleWebSocketMessage(arg, data, length);
        break;
    }
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
        break;
    }
}

void SocketServer::handleWebSocketMessage(void *arg, uint8_t *data, size_t length)
{
    AwsFrameInfo *info = (AwsFrameInfo *)arg;

    const bool validData = info->final &&
                           info->index == 0 && info->len == length &&
                           info->opcode == WS_TEXT;

    if (!validData)
    {
        return;
    }

    data[length] = 0;

    const char *dataChar = reinterpret_cast<char *>(data);
    const bool isMovCoords = strstr(dataChar, ALIAS_SPEED) != nullptr;
    const bool isButtonA = strstr(dataChar, ALIAS_BUTTON_A) != nullptr;

    if (isMovCoords)
    {
        // parse string as a two-members JSON
        const int jsonSize = JSON_OBJECT_SIZE(2);
        StaticJsonDocument<jsonSize> json;
        DeserializationError error = deserializeJson(json, data, length);

        if (error)
        {
            return;
        }

        JoyCoords coords;
        coords.speed = json[ALIAS_SPEED];
        coords.direction = String(json[ALIAS_DIRECTION]);

        _coordsHandler(coords);

        return;
    }

    if (isButtonA)
    {
        const int jsonSize = JSON_OBJECT_SIZE(1);
        StaticJsonDocument<jsonSize> json;
        DeserializationError error = deserializeJson(json, data, length);

        if (error)
        {
            return;
        }

        const bool enabled = json[ALIAS_BUTTON_A];

        _buttonAHandler(enabled);

        return;
    }
}
