# B31DG Assignment 1 - Naomi Solomon

This assignment shows the development of a bare-metal software application that generates two periodically repeating digital signals using an ESP-WROOM-32 module. LEDs and push buttons are used to control and represent the signals.
This has been implemented in both the Arduino IDE and Visual Studio code environment using the Espresssif extension.

Visual Studio Code:
main.c defines the signals and the interrupts for the buttons to trigger the signals when pressed. It includes the configurations of the buttons adn LEDS and conditions for the behaviour of the LEDs.

header.h holds all the definitions and variables for the code seperately anf gets called in main.c.


