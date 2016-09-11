#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include "MKL25Z4.h"

typedef enum Direction {Gpio_Input_e, Gpio_Output_e} Gpio_Direction_e;
typedef enum Logic {Gpio_Low_e, Gpio_High_e} Gpio_Logic_e;

typedef struct Gpio {
    GPIO_Type* port;
    uint8_t pin;
} Gpio_t;

int8_t Gpio_init(Gpio_t* gpio, Gpio_Direction_e dir, Gpio_Logic_e value);

int8_t Gpio_set(Gpio_t* gpio);

int8_t Gpio_clear(Gpio_t* gpio);

int8_t Gpio_toggle(Gpio_t* gpio);

int8_t Gpio_set_output(Gpio_t* gpio, Gpio_Logic_e val);

int8_t Gpio_set_direction(Gpio_t* gpio, Gpio_Direction_e dir);

#endif // GPIO_H
