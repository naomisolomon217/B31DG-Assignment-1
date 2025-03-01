#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG "ESP32"

// GPIO Definitions
#define greenLED GPIO_NUM_32  // Green LED connected to GPIO32
#define redLED GPIO_NUM_33    // Red LED connected to GPIO33
#define enable_button GPIO_NUM_27 // Enable button connected to GPIO27
#define select_button GPIO_NUM_26 // Select button connected to GPIO26

// Define delays
#define syncDelay 50    //synchronisation delay before signal functions
#define debounceDelay 50 //debounce delay for button presses

// Initial pulse-on time
const uint16_t a = 800; // us
// Pulse-off time
const uint16_t b = 1200; // us
// Time between end of final pulse off-time and start of next pulse cycle
const uint16_t d = 6000; // us = 6ms

volatile bool toggleEnable = false;  // Marked as volatile to avoid compiler optimizations
volatile bool toggleSelect = true;   // Marked as volatile

#define debugMode false

#if debugMode
#define timingFactor 1000 //scale delays for dubigging 
#else
#define timingFactor 1 //Normal operation (us)
#endif

// Interrupt Service Routine for select button
static void IRAM_ATTR selectISR(void *arg) {
    esp_rom_delay_us(debounceDelay); // Debounce delay
    if (gpio_get_level(select_button)) //check button press
    {
        toggleSelect = !toggleSelect; //toggle select button
    }
}

// Interrupt Service Routine for enable button
static void IRAM_ATTR enableISR(void *arg) {
    esp_rom_delay_us(debounceDelay); // Debounce delay
    if (gpio_get_level(enable_button)) //check button press
    {
        toggleEnable = !toggleEnable; //toggle enable button
    }
}

//Signal function - increasing pulse signal
void sig() {
    for (int i = a; i <= 1550; i += 50) { //increase pulse-on time by 50 for 16 loops
        gpio_set_level(greenLED, toggleEnable); //Green LED on
        esp_rom_delay_us(i * timingFactor); //pulse-on delay 
        gpio_set_level(greenLED, 0); //Green LED off
        esp_rom_delay_us(b * timingFactor); //pulse-off delay
    }
    esp_rom_delay_us(d * timingFactor); //delay between cycles
}

//Alternative signal funtion - decreasing pulse signal 
void altSig() {
    for (int i = 1550; i >= a; i -= 50) { //decrease pulse-on time by 50 for 16 loops
        gpio_set_level(greenLED, toggleEnable); //green LED on
        esp_rom_delay_us(i * timingFactor); //pulse-on delay 
        gpio_set_level(greenLED, 0); //green LED off
        esp_rom_delay_us(b * timingFactor); //pulse-off delay 
    }
    esp_rom_delay_us(d * timingFactor); //delay between cycles 
}

// Main application loop
void app_main() {
    // Initialize GPIOs
    gpio_reset_pin(enable_button);
    gpio_reset_pin(select_button);
    gpio_reset_pin(redLED);
    gpio_reset_pin(greenLED);

    // Configure buttons as input with pull-ups
    gpio_set_direction(enable_button, GPIO_MODE_INPUT);
    gpio_set_direction(select_button, GPIO_MODE_INPUT);
    gpio_set_pull_mode(enable_button, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(select_button, GPIO_PULLUP_ONLY);

    // Configure interrupts for buttons
    gpio_set_intr_type(enable_button, GPIO_INTR_POSEDGE);  // Trigger on rising edge
    gpio_set_intr_type(select_button, GPIO_INTR_POSEDGE);  // Trigger on rising edge

    // Install ISR service for GPIO interrupts
    gpio_install_isr_service(0);
    gpio_isr_handler_add(enable_button, enableISR, NULL);
    gpio_isr_handler_add(select_button, selectISR, NULL);

    // Configure LEDs as output
    gpio_set_direction(redLED, GPIO_MODE_OUTPUT);
    gpio_set_direction(greenLED, GPIO_MODE_OUTPUT);
    gpio_set_level(redLED, 0);  // Start with LEDs off
    gpio_set_level(greenLED, 0);

    //main loop
    while (1) {
        if (toggleSelect) { //if toggleSelect is true, generate increasing signal 
            gpio_set_level(redLED, 1);  // Turn red LED ON
            esp_rom_delay_us(syncDelay * timingFactor);
            gpio_set_level(redLED, 0);  // Turn red LED OFF
            sig(); //executes increasing signal
        } else { //otherwise generate decreasing signal
            gpio_set_level(redLED, 1);  // Turn red LED ON
            esp_rom_delay_us(syncDelay * timingFactor);
            gpio_set_level(redLED, 0);  // Turn red LED OFF
            altSig(); //executes decreasing signal
        }
    }
}
