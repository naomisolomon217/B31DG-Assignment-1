#ifndef header_h
#define header_h

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
#define syncDelay 50
#define debounceDelay 50

// Initial pulse-on time
extern const uint16_t a;
// Pulse-off time
extern const uint16_t b;
// Time between end of final pulse off-time and start of next pulse cycle
extern const uint16_t d;

extern volatile bool toggleEnable;
extern volatile bool toggleSelect;

#define debugMode false

#if debugMode
#define timingFactor 1000
#else
#define timingFactor 1
#endif

// Function prototypes
void sig();
void altSig();
static void IRAM_ATTR selectISR(void *arg);
static void IRAM_ATTR enableISR(void *arg);
void app_main();

#endif //header.h
