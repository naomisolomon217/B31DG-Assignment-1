# B31DG Assignment 1 - Naomi Solomon

This assignment shows the development of a bare-metal software application that generates two periodically repeating digital signals using an ESP-WROOM-32 module. LEDs and push buttons are used to control and represent the signals.

The normal signal is a continuous loop of 16 pulses, each increasing by a time of 50 microseconds each time before repeating. The alternative signal is the reverse, with the pulses starting longer and decrementing by 50 microseconds with each pulse for 16 pulses. The pulses are shown by a green LED, with a red LED that flashes at the start of each loop. The enable button controls whether the signal is on or off, while the select button flips between the normal signal and the alternative signal.

This has been implemented in both the Arduino IDE and Visual Studio code environment using the Espresssif extension.

## Visual Studio Code:
main.c defines the signals and the interrupts for the buttons to trigger the signals when pressed. It includes the configurations of the buttons adn LEDS and conditions for the behaviour of the LEDs.

header.h holds all the definitions and variables for the code seperately anf gets called in main.c.

## Arduino IDE:
ESass1.ino is the application of the sotware in Arduino, defining two alternate signals, controled by two buttons and represented by LEDs.

