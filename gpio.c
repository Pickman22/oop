#include <stddef.h>
#include "gpio.h"
#include "reg.h"

static GPIO_Type* _gpio_base_ptrs[] = GPIO_BASE_PTRS;
static PORT_Type* _port_base_ptrs[] = PORT_BASE_PTRS;

#define NUMBER_OF_GPIO_PORTS 5

int8_t Gpio_init(Gpio_t* gpio, Gpio_Direction_e dir, Gpio_Logic_e value) {
    uint8_t i;
    int8_t ret = -1;
    for(i = 0U; ((i < NUMBER_OF_GPIO_PORTS) && (gpio != NULL)); ++i)
    {
        if (gpio->port == _gpio_base_ptrs[i]) {
            /* Provided port is valid. */
            ret = 0U;
            /* Configure mux as digital output. */
            _port_base_ptrs[i]->PCR[gpio->pin] = PORT_PCR_MUX(1);
            Gpio_set_direction(gpio, dir);
            Gpio_set_output(gpio, value);
            break;
        }
    }
    return ret;
}

int8_t Gpio_set(Gpio_t* gpio) {
    int8_t ret = -1;
    if((gpio != NULL) && (gpio->pin < 32U)) {
        register32_set_bits(&gpio->port->PSOR, 1U << gpio->pin);
        ret = 0U;
    }
    return ret;
}

int8_t Gpio_clear(Gpio_t* gpio) {
    int8_t ret = -1;
    if((gpio != NULL) && (gpio->pin < 32U)) {
        register32_set_bits(&gpio->port->PCOR, 1U << gpio->pin);
        ret = 0U;
    }
    return ret;
}

int8_t Gpio_toggle(Gpio_t* gpio) {
    int8_t ret = -1;
    if((gpio != NULL) && (gpio->pin < 32U)) {
        register32_set_bits(&gpio->port->PTOR, 1U << gpio->pin);
        ret = 0U;
    }
    return ret;
}

int8_t Gpio_set_output(Gpio_t* gpio, Gpio_Logic_e val) {
    int8_t ret = -1;
    if((gpio != NULL) && (gpio->pin < 32U)) {
        if(val) {
            Gpio_set(gpio);
        } else {
            Gpio_clear(gpio);
        }
    }
    return ret;
}

int8_t Gpio_set_direction(Gpio_t* gpio, Gpio_Direction_e dir) {
    int8_t ret = -1;
    if((gpio != NULL) && (gpio->pin < 32U)) {
        if(dir) {
            register32_set_bits(&gpio->port->PDDR, 1U << gpio->pin);
        } else {
            register32_clear_bits(&gpio->port->PDDR, 1U << gpio->pin);
        }
    }
    return ret;
}
