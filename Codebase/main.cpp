#include <iostream>
#include "gpio.h"
#include "server.h"
#include "logger.h"
#include <chrono>
#include <thread>
#include <string>

using namespace std::chrono_literals;

const int pinOpen = 5;
const int pinClose = 6;
const int opticSensorOpen = 22;
const int opticSensorClose = 13;
const int opticSensorGrip = 26;
const int pwmEnable = 18;

enum GripperState
{
    Open,
    Limbo, // Gripper is neither open nor closed
    Close
};

GripperState getActualState ()
{
    if (io::digitalRead (opticSensorOpen))
        return Open;
    else if (io::digitalRead (opticSensorClose) || io::digitalRead (opticSensorGrip))
        return Close;
    else
        return Limbo;
}

void changeState (GripperState newState, Logger& logger)
{
    if (newState == Limbo)
        throw std::logic_error ("Cannot change state to Limbo");

    if (getActualState () == newState)
        return;

    bool gripperOpen = newState == Open;

    io::digitalWrite (pinOpen, gripperOpen);
    io::digitalWrite (pinClose, !gripperOpen);

    // Start PWM at 1 MHz, with a 50% duty cycle
    io::pwm (pwmEnable, 1000000u, io::DUTY_MAX / 2);

    if (gripperOpen)
    {
        std::cout << "Open" << std::endl;

        logger.logGripperOpen ();
    } else
    {
        std::cout << "Close" << std::endl;

        logger.logGripperClosed ();
    }

    while (getActualState () != newState)
    {
        std::this_thread::sleep_for (100ms);
    }

    io::pwm (pwmEnable, 0, 0); // Turn off PWM

    io::digitalWrite (pinOpen, false);
    io::digitalWrite (pinClose, false);
}

int main(int argc, char* argv[])
{
    io::init ();

    io::pinMode (pinOpen, io::PinMode::Output);
    io::pinMode (pinClose, io::PinMode::Output);
    io::pinMode (pwmEnable, io::PinMode::Output);
    io::pinMode (opticSensorOpen, io::PinMode::Input);
    io::pinMode (opticSensorClose, io::PinMode::Input);
    io::pinMode (opticSensorGrip, io::PinMode::Input);
    
    Logger logger;

    while (true)
    {
        try
        {
            std::cout << "Starting server" << std::endl;

            Server server (54000);

            std::cout << "Waiting for client" << std::endl;

            server.waitForClient ();

            std::cout << "Waiting for input" << std::endl;

            char gripperInput;

            size_t readSize;
            while ((readSize = server.read (&gripperInput, sizeof (gripperInput))) > 0)
            {
                std::cout << "Received input: " << (int)gripperInput << std::endl;

                switch (gripperInput)
                {
                    case 'O':
                        changeState (Open, logger);
                        break;
                    case 'C':
                        changeState (Close, logger);
                        break;
                    case 'N':
                        break;
                    default:
                        std::cerr << "Invalid input" << std::endl;
                }

                const std::string confirm = "<confirmed>\n"; 

                server.write (confirm.c_str (), confirm.length ());
            }

            if (readSize == -1)
                throw std::logic_error ("Socket error.");

            std::cout << "Connection closed." << std::endl;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}
