#include "reg.h"

int8_t register32_write(volatile uint32_t* reg, uint32_t value) {
    int8_t ret = -1;
    if(reg) {
        *reg = value;
        ret = 0;
    }
    return ret;
}

int8_t register32_set_bits(volatile uint32_t* reg, uint32_t mask) {
    int8_t ret = -1;
    uint32_t value;
    if(reg) {
        value = register32_read(reg);
        register32_write(reg, value | mask);
        ret = 0;
    }
    return ret;
}

int8_t register32_clear_bits(volatile uint32_t* reg, uint32_t mask) {
    int8_t ret = -1;
    uint32_t value;
    if(reg) {
        value = register32_read(reg);
        register32_write(reg, value & ~mask);
        ret = 0;
    }
    return ret;
}

uint32_t register32_read(const volatile uint32_t* reg) {
    uint32_t ret = 0;
    if(reg) {
        ret = *reg;
    }
    return ret;
}

int8_t register32_clear_then_set_bits(volatile uint32_t* reg, uint32_t mask,
                                      uint32_t val)
{
    uint32_t ret = -1;
    if(reg) {
        register32_clear_bits(reg, mask);
        register32_set_bits(reg, val);
        ret = 0;
    }
    return ret;
}

int8_t register16_write(volatile uint16_t* reg, uint16_t value) {
    int8_t ret = -1;
    if(reg) {
        *reg = value;
        ret = 0;
    }
    return ret;
}

int8_t register16_set_bits(volatile uint16_t* reg, uint16_t mask) {
    int8_t ret = -1;
    uint16_t value;
    if(reg) {
        value = register16_read(reg);
        register16_write(reg, value | mask);
        ret = 0;
    }
    return ret;
}

int8_t register16_clear_bits(volatile uint16_t* reg, uint16_t mask) {
    int8_t ret = -1;
    uint16_t value;
    if(reg) {
        value = register16_read(reg);
        register16_write(reg, value & ~mask);
        ret = 0;
    }
    return ret;
}

uint16_t register16_read(const volatile uint16_t* reg) {
    uint16_t ret = 0;
    if(reg) {
        ret = *reg;
    }
    return ret;
}

int8_t register16_clear_then_set_bits(volatile uint16_t* reg, uint16_t mask,
                                      uint16_t val)
{
    int8_t ret = -1;
    if(reg) {
        register16_clear_bits(reg, mask);
        register16_set_bits(reg, val);
        ret = 0;
    }
    return ret;
}

int8_t register8_write(volatile uint8_t* reg, uint8_t value) {
    int8_t ret = -1;
    if(reg) {
        *reg = value;
        ret = 0;
    }
    return ret;
}

int8_t register8_set_bits(volatile uint8_t* reg, uint8_t mask) {
    int8_t ret = -1;
    uint8_t value;
    if(reg) {
        value = register8_read(reg);
        register8_write(reg, value | mask);
        ret = 0;
    }
    return ret;
}

int8_t register8_clear_bits(volatile uint8_t* reg, uint8_t mask) {
    int8_t ret = -1;
    uint8_t value;
    if(reg) {
        value = register8_read(reg);
        register8_write(reg, value & ~mask);
        ret = 0;
    }
    return ret;
}

uint8_t register8_read(const volatile uint8_t* reg) {
    uint8_t ret = 0;
    if(reg) {
        ret = *reg;
    }
    return ret;
}

int8_t register8_clear_then_set_bits(volatile uint8_t* reg, uint8_t mask,
                                      uint8_t val)
{
    int8_t ret = -1;
    if(reg) {
        register8_clear_bits(reg, mask);
        register8_set_bits(reg, val);
        ret = 0;
    }
    return ret;
}
