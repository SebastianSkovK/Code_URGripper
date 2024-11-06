#ifndef GPIO_H
#define GPIO_H

namespace io
{
    enum PinMode
    {
        Input = 0,
        Output = 1,
    };

    const int DUTY_MAX = 1000000;
    const int FREQ_MAX = 100000000;

    void init ();

    void terminate ();

    void pinMode (int pin, PinMode mode);

    void digitalWrite (int pin, bool value);

    bool digitalRead (int pin);

    void pwm (int pin, unsigned int freq, int duty);
}
#endif
