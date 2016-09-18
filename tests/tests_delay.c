#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include <stdio.h>
#include "reg.h"
#include "delay.h"
#include "MKL25Z4.h"

uint32_t valid_delay_values[] = {
    0U, 1U, 300U, 4000U, 60000U, 28000U, 30000U, 100U, 3200
};

uint32_t invalid_delay_values[] = {
    0xFFFFFFFF, 65536U
};

static void test_delay_valid_values(void **state) {
    (void)state;
    uint32_t idx;
    for(idx = 0; idx < sizeof(valid_delay_values) / sizeof(uint32_t); idx++) {
        expect_value(register32_set_bits, reg, &SIM->SCGC5);
        expect_value(register32_set_bits, mask, SIM_SCGC5_LPTMR_MASK);

        expect_value(register32_write, reg, &LPTMR0->CSR);
        expect_value(register32_write, val, 0U);

        expect_value(register32_write, reg, &LPTMR0->CMR);
        expect_value(register32_write, val, valid_delay_values[idx]);

        expect_value(register32_clear_then_set_bits, reg, &LPTMR0->PSR);
        expect_value(register32_clear_then_set_bits, mask, LPTMR_PSR_PCS_MASK |
                                       LPTMR_PSR_PBYP_MASK);
        expect_value(register32_clear_then_set_bits, val, LPTMR_PSR_PCS(1U) |
        LPTMR_PSR_PBYP(1U));

        expect_value(register32_set_bits, reg, &LPTMR0->CSR);
        expect_value(register32_set_bits, mask, LPTMR_CSR_TEN_MASK);

        expect_value(register32_read, reg, &LPTMR0->CSR);
        will_return(register32_read, LPTMR_CSR_TCF_MASK);
        delay_ms(valid_delay_values[idx]);
    }
    assert_true(1U);
}

static void test_delay_invalid_values(void **state) {
    (void)state;
    uint32_t idx;
    for(idx = 0; idx < sizeof(invalid_delay_values) / sizeof(uint32_t); idx++) {
        expect_value(register32_set_bits, reg, &SIM->SCGC5);
        expect_value(register32_set_bits, mask, SIM_SCGC5_LPTMR_MASK);

        expect_value(register32_write, reg, &LPTMR0->CSR);
        expect_value(register32_write, val, 0U);

        expect_value(register32_write, reg, &LPTMR0->CMR);
        expect_value(register32_write, val, 0xFFFF);

        expect_value(register32_clear_then_set_bits, reg, &LPTMR0->PSR);
        expect_value(register32_clear_then_set_bits, mask, LPTMR_PSR_PCS_MASK |
                                       LPTMR_PSR_PBYP_MASK);
        expect_value(register32_clear_then_set_bits, val, LPTMR_PSR_PCS(1U) |
        LPTMR_PSR_PBYP(1U));

        expect_value(register32_set_bits, reg, &LPTMR0->CSR);
        expect_value(register32_set_bits, mask, LPTMR_CSR_TEN_MASK);

        expect_value(register32_read, reg, &LPTMR0->CSR);
        will_return(register32_read, LPTMR_CSR_TCF_MASK);
        delay_ms(invalid_delay_values[idx]);
    }
    assert_true(1U);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_delay_valid_values),
        cmocka_unit_test(test_delay_invalid_values),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
