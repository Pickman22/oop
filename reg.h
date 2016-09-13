#ifndef REG_h
#define REG_h

#include <stdint.h>

int8_t register32_write(volatile uint32_t* reg, uint32_t value);

int8_t register32_set_bits(volatile uint32_t* reg, uint32_t mask);

int8_t register32_clear_bits(volatile uint32_t* reg, uint32_t mask);

int8_t register32_clear_then_set_bits(volatile uint32_t* reg, uint32_t mask,
                                      uint32_t val);

uint32_t register32_read(volatile uint32_t* reg);

int8_t register16_write(volatile uint16_t* reg, uint16_t value);

int8_t register16_set_bits(volatile uint16_t* reg, uint16_t mask);

int8_t register16_clear_bits(volatile uint16_t* reg, uint16_t mask);

uint16_t register16_read(volatile uint16_t* reg);

int8_t register16_clear_then_set_bits(volatile uint16_t* reg, uint16_t mask,
                                      uint16_t val);

int8_t register8_write(volatile uint8_t* reg, uint8_t value);

int8_t register8_set_bits(volatile uint8_t* reg, uint8_t mask);

int8_t register8_clear_bits(volatile uint8_t* reg, uint8_t mask);

uint8_t register8_read(volatile uint8_t* reg);

int8_t register8_clear_then_set_bits(volatile uint8_t* reg, uint8_t mask,
                                      uint8_t val);

#endif // REG_H
