#include <stddef.h>
#include "led.h"

#define LED_RED_PIN   18
#define LED_GREEN_PIN 19
#define LED_BLUE_PIN  1

#define LED_RED_PORT   GPIOB
#define LED_GREEN_PORT GPIOB
#define LED_BLUE_PORT  GPIOD

#define LED_DEFAULT_STATE Gpio_High_e

static LED_t _rled = {LED_RED_PORT, LED_RED_PIN};
static LED_t _gled = {LED_GREEN_PORT, LED_GREEN_PIN};
static LED_t _bled = {LED_BLUE_PORT, LED_BLUE_PIN};

LED_t* LED_get_instance(LED_e led) {
    LED_t* ret;
    switch(led) {
        case LED_red_e:
            ret = &_rled;
            Gpio_init((Gpio_t*)&_rled, Gpio_Output_e, LED_DEFAULT_STATE);
        break;

        case LED_blue_e:
            ret = &_bled;
            Gpio_init((Gpio_t*)&_bled, Gpio_Output_e, LED_DEFAULT_STATE);
        break;

        case LED_green_e:
            ret = &_gled;
            Gpio_init((Gpio_t*)&_gled, Gpio_Output_e, LED_DEFAULT_STATE);
        break;

        default:
            ret = NULL;
    }
    return ret;
}

int8_t LED_set_output(LED_t* led, uint8_t val) {
    int8_t ret = -1;
    if(led != NULL) {
        ret = 0;
        if(val) {
            Gpio_set((Gpio_t*)&led);
        } else {
            Gpio_clear((Gpio_t*)&led);
        }
    }
    return ret;
}

int8_t LED_set(LED_t* led) {
    int8_t ret = -1;
    if(led != NULL) {
        ret = 0;
        Gpio_set((Gpio_t*)&led);
    }
    return ret;
}

int8_t LED_clear(LED_t* led) {
    int8_t ret = -1;
    if(led != NULL) {
        ret = 0;
        Gpio_clear((Gpio_t*)&led);
    }
    return ret;
}

int8_t LED_toggle(LED_t* led) {
    int8_t ret = -1;
    if(led != NULL) {
        ret = 0;
        Gpio_toggle((Gpio_t*)&led);
    }
    return ret;
}
