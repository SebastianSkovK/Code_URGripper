#include "gpio.h"
#include <pigpio.h>
#include <iostream>
#include <stdexcept>

void io::init ()
{
    if (gpioInitialise () < 0)
    {
        throw std::logic_error ("GPIO failed to initialize.");
    } else
    {
        std::atexit (io::terminate);
    }
}

void io::terminate ()
{
    gpioTerminate ();
}

void io::pinMode (int pin, PinMode mode)
{
    gpioSetMode (pin, mode);
}

void io::digitalWrite (int pin, bool value)
{
    if (gpioWrite (pin, value) != 0)
        std::cerr << "GPIO write on pin " << pin << " failed.\n";
}

bool io::digitalRead (int pin)
{
    return gpioRead (pin);
}

void io::pwm (int pin, unsigned int freq, int duty)
{
    if (gpioHardwarePWM (pin, freq, duty) != 0)
        std::cerr << "GPIO PWM write on pin " << pin << " failed.\n";
}