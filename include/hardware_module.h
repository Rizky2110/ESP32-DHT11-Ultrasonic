#ifndef hardware_module_h
#define hardware_module_h

#include <Arduino.h>

class HARDWARE
{
public:
    // define pin
    const int pDataDhtSensor = 4;
    const int pTrigerUltrasonik = 5;
    const int pEchoUltrasonik = 18;

    void setup_hardware();
};

#endif
