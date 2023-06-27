#include "../include/hardware_module.h"

void HARDWARE::setup_hardware()
{
    pinMode(pTrigerUltrasonik, OUTPUT);
    pinMode(pEchoUltrasonik, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);
}
