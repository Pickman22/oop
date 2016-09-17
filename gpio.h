#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include "MKL25Z4.h"
#include "pins.h"

typedef enum Direction {Gpio_Input_e, Gpio_Output_e} Gpio_Direction_e;
typedef enum Logic {Gpio_Low_e, Gpio_High_e} Gpio_Logic_e;

typedef PinName Gpio_t;

int8_t Gpio_init(PinName pin, Gpio_Direction_e dir, Gpio_Logic_e value);

int8_t Gpio_set(PinName pin);

int8_t Gpio_clear(PinName pin);

int8_t Gpio_toggle(PinName pin);

int8_t Gpio_set_output(PinName pin, Gpio_Logic_e val);

int8_t Gpio_set_direction(PinName pin, Gpio_Direction_e dir);

#endif // GPIO_H
