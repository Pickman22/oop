#include "reg.h"
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>

int8_t register32_write(volatile uint32_t* reg, uint32_t value) {
    check_expected(reg);
    check_expected(value);
    return 0;
}

int8_t register32_set_bits(volatile uint32_t* reg, uint32_t mask) {
    printf("[Reg32] Setting bits: 0x%08x to register: 0x%08lx\n\r", mask, (uintptr_t)reg);
    check_expected(reg);
    check_expected(mask);
    return 0;
}

int8_t register32_clear_bits(volatile uint32_t* reg, uint32_t mask) {
    check_expected(reg);
    check_expected(mask);
    return 0;
}

uint32_t register32_read(volatile uint32_t* reg) {
    check_expected(reg);
    return mock_type(uint32_t);
}

int8_t register16_write(volatile uint32_t* reg, uint16_t value) {
    check_expected(reg);
    check_expected(value);
    return 0;
}

int8_t register16_set_bits(volatile uint32_t* reg, uint16_t mask) {
}

int8_t register16_clear_bits(volatile uint32_t* reg, uint16_t mask) {
    check_expected(reg);
    check_expected(mask);
    return 0;
}

uint16_t register16_read(volatile uint32_t* reg) {
    check_expected(reg);
    return mock_type(uint16_t);
}

int8_t register8_write(volatile uint8_t* reg, uint8_t value) {
    check_expected(reg);
    check_expected(value);
    return 0;
}

int8_t register8_set_bits(volatile uint8_t* reg, uint8_t mask) {
    printf("[Reg8] Setting bits: 0x%08x to register: 0x%08lx\n\r", mask, (uintptr_t)reg);
    check_expected(reg);
    check_expected(mask);
    return 0;
}

int8_t register8_clear_bits(volatile uint8_t* reg, uint8_t mask) {
    check_expected(reg);
    check_expected(mask);
    return 0;
}

uint8_t register8_read(volatile uint8_t* reg) {
    check_expected(reg);
    return mock_type(uint8_t);
}
