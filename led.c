#include <stddef.h>
#include "led.h"

#define LED_DEFAULT_STATE Gpio_High_e

int8_t LED_init(LED_e led) {
    int8_t ret = -1;
    switch(led) {
        case LED_red_e:
            ret = Gpio_init(led, Gpio_Output_e, LED_DEFAULT_STATE);
        break;

        case LED_blue_e:
            ret = Gpio_init(led, Gpio_Output_e, LED_DEFAULT_STATE);
        break;

        case LED_green_e:
            ret = Gpio_init(led, Gpio_Output_e, LED_DEFAULT_STATE);
        break;

        default:
            ret = -1;
    }
    return ret;
}

int8_t LED_set_output(LED_e led, uint8_t val) {
    return val ? Gpio_set(led) : Gpio_clear(led);
}

int8_t LED_set(LED_e led) {
    return Gpio_clear(led);
}

int8_t LED_clear(LED_e led) {
    return Gpio_set(led);
}

int8_t LED_toggle(LED_e led) {
    return Gpio_toggle(led);
}
