
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_rom_sys.h"

#define TAG "ESP32"

// GPIO Definitions
#define greenLED GPIO_NUM_32  // green LED connected to GPIO32
#define redLED GPIO_NUM_33    // red LED connected to GPIO33
#define enable_button GPIO_NUM_27 // enable button connected to GPIO27
#define select_button GPIO_NUM_26 // select button connected to GPIO26

// Define delays
#define syncDelay 50
#define debounceDelay 50

// Initial pulse-on time
const uint16_t a = 800; //us
// Pulse-off time
const uint16_t b = 1200; //us
// Number of pulses
const uint16_t c = 16;
// Time between end of final pulse off-time and start of next pulse cycle
const uint16_t d = 6000; //us = 6ms

bool toggleEnable = false;
bool toggleSelect = true;

#define debugMode true

#if debugMode
#define timingFactor 1000
#else
#define timingFactor 1
#endif

// Interrupt Service Routines
static void IRAM_ATTR selectISR(void *arg) {
    esp_rom_delay_us(debounceDelay);
    if (gpio_get_level(select_button)) {
        toggleSelect = !toggleSelect;
        printf("select");
    }
}

static void IRAM_ATTR enableISR(void *arg) {
    esp_rom_delay_us(debounceDelay);
    if (gpio_get_level(enable_button)) {
        toggleEnable = !toggleEnable;
        printf("enable");
    }
}

// Signal functions
void sig() {
    for (int i = a; i <= 1550; i += 50) {
        gpio_set_level(greenLED, toggleEnable);
        esp_rom_delay_us(i * timingFactor);
        gpio_set_level(greenLED, 0);
        esp_rom_delay_us(b * timingFactor);
    }
    esp_rom_delay_us(d * timingFactor);
}

void altSig() {
    for (int i = 1550; i >= a; i -= 50) {
        gpio_set_level(greenLED, toggleEnable);
        esp_rom_delay_us(i * timingFactor);
        gpio_set_level(greenLED, 0);
        esp_rom_delay_us(b * timingFactor);
    }
    esp_rom_delay_us(d * timingFactor);
}

// Main application loop
void app_main() {
    // Initialize GPIO
    gpio_set_direction(enable_button, GPIO_MODE_INPUT);
    gpio_set_direction(select_button, GPIO_MODE_INPUT);
    gpio_set_pull_mode(enable_button, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(select_button, GPIO_PULLUP_ONLY);
    
    gpio_set_direction(redLED, GPIO_MODE_OUTPUT);
    gpio_set_direction(greenLED, GPIO_MODE_OUTPUT);
    
    // Install ISR service for GPIO interrupts
    gpio_install_isr_service(0);
    gpio_isr_handler_add(enable_button, enableISR, NULL);
    gpio_isr_handler_add(select_button, selectISR, NULL);

    while (1) {
        if (toggleSelect) {
            gpio_set_level(redLED, 1);
            esp_rom_delay_us(syncDelay * timingFactor);
            gpio_set_level(redLED, 0);
            sig();
        } else {
            gpio_set_level(redLED, 1);
            esp_rom_delay_us(syncDelay * timingFactor);
            gpio_set_level(redLED, 0);
            altSig();
        }
    }
}

