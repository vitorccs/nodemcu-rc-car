#include <Arduino.h>
#include <DigitalLed.h>
#include <DCMotor.h>
#include <Car.h>
#include <WebJoystickHandler.h>
#include <ArduinoJson.h>
#include <WifiHandler.h>
#include <SocketServer.h>
#include <ESPAsyncWebServer.h>
#include <JoyCoords.h>
#include <SoftwareSerial.h>

#define WIFI_SSID "YOUR_SSID"
#define WIFI_PWD "YOUR_PWD"
#define JOYSTICK_DEBUG true
#define PIN_FLED D6
#define PIN_M1_EN D1
#define PIN_M1_IN1 D0
#define PIN_M1_IN2 D2
#define PIN_M2_EN D5
#define PIN_M2_IN1 D3
#define PIN_M2_IN2 D4
#define MIN_MOTOR_SPEED 80 // (between 0 to 255)

// Car components
DCMotor motor1(PIN_M1_EN, PIN_M1_IN1, PIN_M1_IN2);
DCMotor motor2(PIN_M2_EN, PIN_M2_IN1, PIN_M2_IN2);
DigitalLed fLed(PIN_FLED);
Car car(motor1, motor2, fLed);

// Handlers
WifiHandler wifiHandler;
WebJoystickHandler webJoystickHandler(car);

// Socket server
SocketServer socketServer;

void setup()
{
  Serial.begin(115200);

  car.stop();
  car.setMinAbsSpeed(MIN_MOTOR_SPEED);

  wifiHandler.connect(WIFI_SSID, WIFI_PWD);

  // Set Web Joystick (Web sockets)
  webJoystickHandler.setDebug(JOYSTICK_DEBUG);

  CoordsHandlerFunction coordsHandler = [&](JoyCoords coords)
  {
    webJoystickHandler.handle(coords);
  };

  ButtonAToggleHandlerFunction buttonAHandler = [&](bool toggle)
  {
    webJoystickHandler.toggleFrontLights(toggle);
  };

  socketServer.init(coordsHandler,
                    buttonAHandler);
}

void loop()
{
}
