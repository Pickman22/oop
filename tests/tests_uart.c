#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include <stdio.h>
#include "uart.h"
#include "reg.h"
#include "MKL25Z4.h"

typedef struct _uart_map {
    uint32_t rx_name;
    uint32_t tx_name;
    volatile uint8_t* uart_register;
    uint32_t mux;
    volatile uint32_t* port_register;
    uint32_t rx;
    uint32_t tx;
    uint32_t mask;
} _uart_map_t;

static const _uart_map_t _valid_uart_map[] = {
    {PTE1,  PTE0,  (volatile uint8_t*)UART1_BASE,   3U, (volatile uint32_t*)PORTE_BASE, 1U,  0U,  SIM_SCGC4_UART1_MASK},
    {PTE21, PTE20, (volatile uint8_t*)UART0_BASE,   4U, (volatile uint32_t*)PORTE_BASE, 21U, 20U, SIM_SCGC4_UART0_MASK},
    {PTE23, PTE22, (volatile uint8_t*)UART2_BASE,   4U, (volatile uint32_t*)PORTE_BASE, 23U, 22U, SIM_SCGC4_UART2_MASK},
    {PTA1,  PTA2,  (volatile uint8_t*)UART0_BASE,   2U, (volatile uint32_t*)PORTA_BASE, 1U,  2U,  SIM_SCGC4_UART0_MASK},
    {PTA15, PTA14, (volatile uint8_t*)UART0_BASE,   3U, (volatile uint32_t*)PORTA_BASE, 15U, 14U, SIM_SCGC4_UART0_MASK},
    {PTA18, PTA19, (volatile uint8_t*)UART1_BASE,   3U, (volatile uint32_t*)PORTA_BASE, 18U, 19U, SIM_SCGC4_UART1_MASK},
    {PTB16, PTB17, (volatile uint8_t*)UART0_BASE,   3U, (volatile uint32_t*)PORTA_BASE, 16U, 17U, SIM_SCGC4_UART0_MASK},
    {PTC3,  PTC4,  (volatile uint8_t*)UART1_BASE,   3U, (volatile uint32_t*)PORTC_BASE, 3U,  4U,  SIM_SCGC4_UART1_MASK},
    {PTD2,  PTD3,  (volatile uint8_t*)UART2_BASE,   2U, (volatile uint32_t*)PORTD_BASE, 2U,  3U,  SIM_SCGC4_UART2_MASK},
    {PTD4,  PTD5,  (volatile uint8_t*)UART2_BASE,   2U, (volatile uint32_t*)PORTD_BASE, 4U,  5U,  SIM_SCGC4_UART2_MASK},
    {PTD6,  PTD7,  (volatile uint8_t*)UART0_BASE,   0U, (volatile uint32_t*)PORTD_BASE, 6U,  7U,  SIM_SCGC4_UART0_MASK},
};

#define VALID_UART_MAP_SIZE (sizeof(_valid_uart_map) / sizeof(_uart_map_t))

static void test_null_arguments_returns_error(void **state) {
    assert_int_equal(Uart_init(NULL, NULL), -1);
}

static void test_uart_init(void **state) {
    Uart_conf_t params;
    Uart_t* uart;
    uint32_t bcr;
    Uart_get_default_conf(&params);
    params.baud_rate = Baud_115200_e;
    bcr = 0x1FFF & (24000000U / params.baud_rate / 16U);
    uint8_t idx;
    uint32_t pin;
    PORT_Type* port;
    for(idx = 0; idx < VALID_UART_MAP_SIZE; ++idx) {
        params.rx = _valid_uart_map[idx].rx_name;
        params.tx = _valid_uart_map[idx].tx_name;
        port = (PORT_Type*)_valid_uart_map[idx].port_register;
        uart = (Uart_t*)_valid_uart_map[idx].uart_register;

        /* Enable UART clock */
        expect_value(register32_set_bits, reg, &SIM->SCGC4);
        expect_value(register32_set_bits, mask, _valid_uart_map[idx].mask);

        /* Configure MUX for Tx/Rx */
        pin = _valid_uart_map[idx].rx;
        if(uart == (Uart_t*)UART0_BASE)
        {
            expect_value(register32_clear_then_set_bits, reg, &SIM->SOPT2);
            expect_value(register32_clear_then_set_bits,
                         mask, SIM_SOPT2_UART0SRC_MASK);
            expect_value(register32_clear_then_set_bits, val,
                         SIM_SOPT2_UART0SRC(1));

            expect_value(register8_clear_then_set_bits, reg, &uart->C4);
            expect_value(register8_clear_then_set_bits,
                         mask, UARTLP_C4_OSR_MASK);
            expect_value(register8_clear_then_set_bits, val,
                         UARTLP_C4_OSR(SystemCoreClock-1));
            bcr = (SystemCoreClock / UART0_OSR) / params.baud_rate;

        } else {
            bcr = (SystemCoreClock / UART0_OSR) / params.baud_rate;
        }
        pin = _valid_uart_map[idx].tx;
        expect_value(register32_clear_then_set_bits, reg, &port->PCR[pin]);
        expect_value(register32_clear_then_set_bits, val, PORT_PCR_MUX(_valid_uart_map[idx].mux));
        expect_value(register32_clear_then_set_bits, mask, PORT_PCR_MUX_MASK);

        pin = _valid_uart_map[idx].rx;
        expect_value(register32_clear_then_set_bits, reg, &port->PCR[pin]);
        expect_value(register32_clear_then_set_bits, val, PORT_PCR_MUX(_valid_uart_map[idx].mux));
        expect_value(register32_clear_then_set_bits, mask, PORT_PCR_MUX_MASK);

        expect_value(register8_write, reg, &uart->C2);
        expect_value(register8_write, val, 0U);

        expect_value(register8_write, reg, &uart->C1);
        expect_value(register8_write, val, 0U);

        expect_value(register8_write, reg, &uart->C3);
        expect_value(register8_write, val, 0U);

        expect_value(register8_write, reg, &uart->S2);
        expect_value(register8_write, val, 0U);

        /* Configure Baud Rate. */
        expect_value(register8_clear_then_set_bits, reg, &uart->BDH);
        expect_value(register8_clear_then_set_bits, mask, UART_BDH_SBR_MASK);
        expect_value(register8_clear_then_set_bits, val, (uint8_t)(bcr >> 0x08U));

        expect_value(register8_clear_then_set_bits, reg, &uart->BDL);
        expect_value(register8_clear_then_set_bits, mask, UART_BDL_SBR_MASK);
        expect_value(register8_clear_then_set_bits, val, (uint8_t)bcr);

        expect_value(register8_set_bits, reg, &uart->C2);
        expect_value(register8_set_bits, mask, UART_C2_RE_MASK |
                     UART_C2_TE_MASK | UART_C2_RIE_MASK);
        assert_int_equal(0, Uart_init(uart, &params));
    }
}

static void test_invalid_uart_returns_error(void **state) {
    int i;
    Uart_conf_t params = {0};
    Uart_t uart;
    for(i = 0; i < VALID_UART_MAP_SIZE; ++i) {
        params.rx = _valid_uart_map[i].tx_name;
        params.tx = _valid_uart_map[i].rx_name;
        assert_int_equal(-1, Uart_init(&uart, &params));
    }
}

static void test_uart_putc_returns_error(void **state) {

}

static void test_uart_putc_returns_no_error(void **state) {

}

static void test_uart_getc_returns_error(void **state) {

}

static void test_uart_getc_returns_no_error(void **state) {

}

static void test_uart_gets_returns_error(void **state) {

}

static void test_uart_gets_returns_no_error(void **state) {
    
}

static void test_uart_puts_returns_error(void **state) {

}

static void test_uart_puts_returns_no_error(void **state) {

}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_null_arguments_returns_error),
        cmocka_unit_test(test_uart_init),
        cmocka_unit_test(test_invalid_uart_returns_error),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
