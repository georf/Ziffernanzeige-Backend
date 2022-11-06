#include <LedDriver.h>

LedDriver::LedDriver(u_int8_t PIN, LedState defaultValue, bool invertedValue)
{
    pin = PIN;
    inverted = invertedValue;
    pinMode(pin, OUTPUT);
    set(defaultValue);
}

void LedDriver::work()
{
    bool value;
    switch (state)
    {
    case off:
        value = LOW;
        break;
    case on:
        value = HIGH;
        break;
    case slow_short_blink:
        value = (millis() % 3000 / 1000 == 0) && (millis() % 1000 / 100 == 1);
        break;
    case blink:
        value = millis() / 1000 % 2;
        break;
    case fast_blink:
        value = millis() / 500 % 2;
        break;
    case blink_3_times:
        value = millis() / 200 % 2;
        if (lastChangeTime + 3000 < millis())
            state = stateAfter;
        break;
    case shot:
        value = lastChangeTime > millis() - 15;
        if (!value)
            state = stateAfter;
        break;
    }
    digitalWrite(pin, value ^ inverted);
}

void LedDriver::set(LedState newValue)
{
    set(newValue, off);
}

void LedDriver::set(LedState newValue, LedState afterValue)
{
    state = newValue;
    stateAfter = afterValue;
    lastChangeTime = millis();
    work();
}