#include <stddef.h>
#include "gpio.h"

//static uint32_t _gpio_base_regs[] = GPIO_BASE_ADDRS;
static GPIO_Type* _gpio_base_ptrs[] = GPIO_BASE_PTRS;
static PORT_Type* _port_base_ptrs[] = PORT_BASE_PTRS;

#define NUMBER_OF_GPIO_PORTS 5

//CLOCK_EnableClock(kCLOCK_PortB);
//CLOCK_EnableClock(kCLOCK_PortD);

int8_t Gpio_init(Gpio_t* gpio, Gpio_Direction_e dir, Gpio_Logic_e value) {
    uint8_t i;
    int8_t ret = -1;
    for(i = 0; ((i < NUMBER_OF_GPIO_PORTS) && (gpio != NULL)); ++i)
    {
        if (gpio->port == _gpio_base_ptrs[i]) {
            /* Provided port is valid. */
            ret = 0;
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
    if((gpio != NULL) && (gpio->pin < 32)) {
        gpio->port->PSOR |= (1 << gpio->pin);
        ret = 0;
    }
    return ret;
}

int8_t Gpio_clear(Gpio_t* gpio) {
    int8_t ret = -1;
    if((gpio != NULL) && (gpio->pin < 32)) {
        gpio->port->PCOR |= (1 << gpio->pin);
        ret = 0;
    }
    return ret;
}

int8_t Gpio_toggle(Gpio_t* gpio) {
    int8_t ret = -1;
    if((gpio != NULL) && (gpio->pin < 32)) {
        gpio->port->PTOR |= (1 << gpio->pin);
        ret = 0;
    }
    return ret;
}

int8_t Gpio_set_output(Gpio_t* gpio, Gpio_Logic_e val) {
    int8_t ret = -1;
    if((gpio != NULL) && (gpio->pin < 32)) {
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
    if((gpio != NULL) && (gpio->pin < 32)) {
        if(dir) {
            gpio->port->PDDR |= (1 << (uint32_t)gpio->pin);
        } else {
            gpio->port->PDDR &= ~(1 << (uint32_t)gpio->pin);
        }
    }
    return ret;
}
