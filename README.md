# NodeMCU RC Car
Remote Controlled Car using NodeMCU ESP8266-12 V2 board. 

<img src="https://github.com/vitorccs/nodemcu-rc-car/assets/9891961/9d004922-335e-4b96-a849-a66e0d9e6d0b" width="400">

<img src="https://github.com/vitorccs/nodemcu-rc-car/assets/9891961/57368ac1-5af6-4f35-8e32-06bb7ed8ab99" width="400">


## Description
![BluetoohRcCar_bb](https://github.com/vitorccs/nodemcu-rc-car/assets/9891961/9ca79c8e-4f59-4822-86c7-91603ebf01bb)


This is a [Platform IO IDE](https://platformio.org/platformio-ide) project coded in C++. 

The car is controlled by a Virtual Joystick in a Web page. The NodeMCU connects to your Wi-Fi network and provides a local IP where the Virtual Joystick can be found.

<img src="https://github.com/vitorccs/nodemcu-rc-car/assets/9891961/90fd7ba1-2214-45f5-a3a1-4691bbeec021" width="300">


## Components
* 01 - Car Chassis  (can be 2WD or 4WD)
* 02 - DC Motors (3v - 6v)
* 01 - NodeMCU v2
* 01 - L298N Dual H-Bridge board
* 02 - White Leds
* 04 - 100 Ω Resistors
* 02 - 18650 batteries (3.7v - 4.2v)
* 01 - Battery support

## About PlatformIO IDE
Platform IO is a plugin for Microsoft Virtual Studio Code. It is a more robust IDE compared to official Arduino IDE. It also allows us to easily create our own private libraries and use a more object oriented code.

## About the code
The PINs can be customized in the `main.cpp` 
```c++
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

```

Fine-tuning customizations can be done in the individual files like `DCMotor.h` for changing speed parameters
```c++
#ifndef DCMOTOR_H
#define DCMOTOR_H
#include <Arduino.h>

class DCMotor
{
public:
    DCMotor(uint8_t pinEn, uint8_t pinIn1, uint8_t pinIn2);
    void backward(uint8_t speed = 100);
    void forward(uint8_t speed = 100);
    void setMinAbsSpeed(uint8_t absSpeed);
    void stop();

private:
    uint8_t pinEn;
    uint8_t pinIn1;
    uint8_t pinIn2;
    uint8_t absSpeed = 0;
    uint8_t maxAbsSpeed = 255;
    uint8_t minAbsSpeed = 50;
    uint8_t ignoreAbsSpeed = 30;

    void setSpeed(uint8_t speed);
};

#endif
```
## About Car Chassis
This project can work with a 2WD or 4WD car chassis like these ones:

### Reference 1 (International):
<img src="https://github.com/vitorccs/nodemcu-rc-car/assets/9891961/ab262906-3d15-41b7-8fab-b9664d6263a4" width="300">

https://www.aliexpress.us/item/3256805855273192.html


### Reference 2 (International):
<img src="https://github.com/vitorccs/nodemcu-rc-car/assets/9891961/6e92b8e4-c452-4b1f-b8ce-b5e51e591bed" width="300">

https://www.aliexpress.us/item/3256801542172576.html

### Reference 3 (Brazil):
<img src="https://github.com/vitorccs/nodemcu-rc-car/assets/9891961/a79de4b0-ea64-42f6-b933-a801766820ab" width="300">

https://www.robocore.net/robotica-robocore/plataforma-robotica-rocket-tank


## Fritzing file
The eletronic schematic was created in the [Fritzing](https://fritzing.org/) software and can be downloaded at
[NodeMcuRcCar.zip](https://github.com/vitorccs/nodemcu-rc-car/files/12888743/NodeMcuRcCar.zip)

