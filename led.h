#ifndef LED_H
#define LED_H

#include "gpio.h"
#include "MKL25Z4.h"
#include "pins.h"

typedef enum LED_enum {LED_red_e = LED_RED,
    LED_blue_e = LED_BLUE, LED_green_e = LED_GREEN
} LED_e;

int8_t LED_init(LED_e led);

int8_t LED_set_output(LED_e led, uint8_t val);

int8_t LED_set(LED_e led);

int8_t LED_clear(LED_e led);

int8_t LED_toggle(LED_e led);

#endif // LED_H
