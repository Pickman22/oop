#include <stddef.h>
#include "gpio.h"
#include "reg.h"

static GPIO_Type* _gpio_base_ptrs[] = GPIO_BASE_PTRS;
static PORT_Type* _port_base_ptrs[] = PORT_BASE_PTRS;

#define NUMBER_OF_GPIO_PORTS 5

int8_t Gpio_init(PinName pin_name, Gpio_Direction_e dir, Gpio_Logic_e value) {
    int8_t ret = -1;
    uint8_t i = MAP_PIN_TO_PORT_INDEX(pin_name);
    uint8_t pin_number = MAP_PINNAME_TO_NUMBER(pin_name);
    if(i < NUMBER_OF_GPIO_PORTS) {
        PORT_Type* port = _port_base_ptrs[i];
        port->PCR[pin_number] = PORT_PCR_MUX(1U);
        Gpio_set_direction(pin_name, dir);
        Gpio_set_output(pin_name, value);
        ret = 0U;
    }
    return ret;
}

int8_t Gpio_set(PinName pin) {
    int8_t ret = -1;
    uint8_t i = MAP_PIN_TO_PORT_INDEX(pin);
    uint8_t pin_number = MAP_PINNAME_TO_NUMBER(pin);
    GPIO_Type* port = _gpio_base_ptrs[i];
    if(i < NUMBER_OF_GPIO_PORTS) {
        register32_set_bits(&port->PSOR, 1U << pin_number);
        ret = 0U;
    }
    return ret;
}

int8_t Gpio_clear(PinName pin) {
    int8_t ret = -1;
    uint8_t i = MAP_PIN_TO_PORT_INDEX(pin);
    uint8_t pin_number = MAP_PINNAME_TO_NUMBER(pin);
    GPIO_Type* port = _gpio_base_ptrs[i];
    if(i < NUMBER_OF_GPIO_PORTS) {
        register32_set_bits(&port->PCOR, 1U << pin_number);
        ret = 0U;
    }
    return ret;
}

int8_t Gpio_toggle(PinName pin) {
    int8_t ret = -1;
    uint8_t i = MAP_PIN_TO_PORT_INDEX(pin);
    uint8_t pin_number = MAP_PINNAME_TO_NUMBER(pin);
    GPIO_Type* port = _gpio_base_ptrs[i];
    if(i < NUMBER_OF_GPIO_PORTS) {
        register32_set_bits(&port->PTOR, 1U << pin_number);
        ret = 0U;
    }
    return ret;
}

int8_t Gpio_set_output(PinName pin, Gpio_Logic_e val) {
    return val ? Gpio_set(pin) : Gpio_clear(pin);
}

int8_t Gpio_set_direction(PinName pin, Gpio_Direction_e dir) {
    int8_t ret = -1;
    uint8_t i = MAP_PIN_TO_PORT_INDEX(pin);
    uint8_t pin_number = MAP_PINNAME_TO_NUMBER(pin);
    GPIO_Type* port = _gpio_base_ptrs[i];
    if(i < NUMBER_OF_GPIO_PORTS) {
        ret = dir ? register32_set_bits(&port->PDDR, 1U << pin_number) :
                    register32_clear_bits(&port->PDDR, 1U << pin_number);
    }
    return ret;
}
