#include "Arduino.h"

enum LedState
{
    off,
    on,
    shot,
    slow_short_blink,
    blink,
    blink_3_times,
    fast_blink
};

class LedDriver
{
private:
    unsigned long lastChangeTime;
    bool inverted;

public:
    uint8_t pin;
    LedState state;
    LedState stateAfter;
    void work();
    void set(LedState newValue);
    void set(LedState newValue, LedState afterValue);
    LedDriver(u_int8_t PIN, LedState defaultValue, bool inverted);
};
