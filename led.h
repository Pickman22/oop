#ifndef LED_H
#define LED_H

#include "gpio.h"
#include "MKL25Z4.h"

typedef enum LED_enum {LED_red_e, LED_blue_e, LED_green_e} LED_e;

typedef Gpio_t LED_t;

LED_t* LED_get_instance(LED_e led);

int8_t LED_set_output(LED_t* led, uint8_t val);

int8_t LED_set(LED_t* led);

int8_t LED_clear(LED_t* led);

int8_t LED_toggle(LED_t* led);

#endif // LED_H
