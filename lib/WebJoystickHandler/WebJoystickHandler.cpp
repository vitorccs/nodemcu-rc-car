#include <Arduino.h>
#include <Car.h>
#include <WebJoystickHandler.h>
#include <JoyCoords.h>

WebJoystickHandler::WebJoystickHandler(Car &car) : _car(car)
{
    this->_car = car;
}

void WebJoystickHandler::setDebug(bool enable)
{
    this->debug = enable;
}

void WebJoystickHandler::handle(JoyCoords coords)
{
    const int16_t angle = coords.angle;
    const int16_t speed = coords.speed;

    const bool isForward = (angle >= 170) && (angle <= 190);

    if (isForward)
    {
        debugMovement(coords, "Forward");
        this->_car.forward(speed);
        return;
    }

    const bool isBackward = (angle >= 0 && angle <= 10) || (angle >= 350 && angle <= 360);

    if (isBackward)
    {
        debugMovement(coords, "Backward");
        this->_car.backward(speed);
        return;
    }

    const bool isLeft = (angle >= 260) && (angle <= 280);

    if (isLeft)
    {
        debugMovement(coords, "Left");
        this->_car.turn(speed, 0);
        return;
    }

    const bool isRight = (angle >= 80) && (angle <= 100);

    if (isRight)
    {
        debugMovement(coords, "Right");
        this->_car.turn(0, speed);
        return;
    }

    const bool isFowardLeft = (angle > 190) && (angle < 260);

    if (isFowardLeft)
    {
        debugMovement(coords, "Forward Left");
        this->_car.forwardLeft(speed);
        return;
    }

    const bool isFowardRight = (angle > 100) && (angle < 170);

    if (isFowardRight)
    {
        debugMovement(coords, "Forward Right");
        _car.forwardRight(speed);
        return;
    }

    const bool isBackwardLeft = (angle > 280) && (angle < 350);

    if (isBackwardLeft)
    {
        debugMovement(coords, "Backward Left");
        this->_car.backwardLeft(speed);
        return;
    }

    const bool isBackwardRight = (angle > 10) && (angle < 80);

    if (isBackwardRight)
    {
        debugMovement(coords, "Backward Right");
        this->_car.backwardRight(speed);
        return;
    }

    debugMovement(coords, "Stop");
    this->_car.stop();
}

void WebJoystickHandler::toggleFrontLights(bool enable)
{
    if (enable)
    {
        this->_car.frontLightsOn();
    }
    else
    {
        this->_car.frontLightsOff();
    }
}

void WebJoystickHandler::debugMovement(JoyCoords coords, String direction)
{
    if (!this->debug)
    {
        return;
    }

    Serial.print("(speed: ");
    Serial.print(coords.speed);
    Serial.print(", angle: ");
    Serial.print(coords.angle);
    Serial.println(") " + direction);
}
