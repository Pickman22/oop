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
        printf("New configuration...\n\r");

        /* Enable UART clock */
        expect_value(register32_set_bits, reg, &SIM->SCGC4);
        expect_value(register32_set_bits, mask, _valid_uart_map[idx].mask);

        /* Configure MUX for Tx/Rx */
        pin = _valid_uart_map[idx].rx;
        expect_value(register32_set_bits, reg, &port->PCR[pin]);
        expect_value(register32_set_bits, mask, PORT_PCR_MUX(_valid_uart_map[idx].mux));
        pin = _valid_uart_map[idx].tx;
        expect_value(register32_set_bits, reg, &port->PCR[pin]);
        expect_value(register32_set_bits, mask, PORT_PCR_MUX(_valid_uart_map[idx].mux));

        /* Enable Tx/Rx */
        expect_value(register8_set_bits, mask, UART_C2_TE_MASK | UART_C2_RE_MASK);
        expect_value(register8_set_bits, reg, &uart->C2);

        /* Compute BUS clock value. */
        expect_value(register32_read, reg, &SIM->CLKDIV1);
        will_return(register32_read, 0x10000); // Div value.

        /* Configure Baud Rate. */
        expect_value(register8_write, reg, &uart->BDH);
        expect_value(register8_write, value, (uint8_t)(bcr >> 0x08U));

        expect_value(register8_write, reg, &uart->BDL);
        expect_value(register8_write, value, (uint8_t)bcr);

        /* Configure Parity. */
        expect_value(register8_clear_bits, reg, &uart->C1);
        expect_value(register8_clear_bits, mask, UART_C1_PT_MASK); // Parity type.

        expect_value(register8_clear_bits, reg, &uart->C1);
        expect_value(register8_clear_bits, mask, UART_C1_PE_MASK); // Parity mode.
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

static void test_uart_set_baud_rate(void **state) {
    Baud_Rate_t valid_rates[] = {
        Baud_300_e,
        Baud_4800_e,
        Baud_115200_e,
        Baud_19200_e
    };
    uint8_t idx;
    uint32_t bcr;
    Uart_t* uart = (Uart_t*)UART1_BASE;
    will_return_always(register32_read, 0x10000);
    for(idx = 0; idx < sizeof(valid_rates) / sizeof(Baud_Rate_t); ++idx) {
        bcr = 0x1FFF & (24000000U / valid_rates[idx] / 16U);
        expect_value(register8_write, value, (uint8_t)(bcr >> 0x08U));
        expect_value(register8_write, value, (uint8_t)bcr);
        assert_int_equal(0, Uart_set_baud_rate(uart, valid_rates[idx]));
    }
    /* Test invalid baud rate configurations. */
    assert_int_equal(-1, Uart_set_baud_rate(uart, 0));
    assert_int_equal(-1, Uart_set_baud_rate(uart, 0xFFFFFFFF));
}

int main(void) {
    const struct CMUnitTest tests[] = {
        //cmocka_unit_test(test_null_arguments_returns_error),
        cmocka_unit_test(test_uart_init),
        //cmocka_unit_test(test_invalid_uart_returns_error),
        //cmocka_unit_test(test_uart_set_baud_rate),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
