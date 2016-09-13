#include "uart.h"
#include "reg.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define UART0_OSR 16U
#define UART_SBR_MAX 8191U

static const Uart_conf_t _uart_defualt_conf = {
    .bit_mode = Mode_8bits_e,
    .stop_bits = Stop_1bit_e,
    .baud_rate = Baud_9600_e,
    .parity_mode = Parity_Even_e,
    .parity_type = Parity_Disabled_e,
    .tx = PTA2,
    .rx = PTA1,
};

typedef struct _uart_map {
    uint32_t rx_name;
    uint32_t tx_name;
    volatile uint32_t* uart_register;
    uint32_t mux;
    volatile uint32_t* port_register;
    uint32_t rx;
    uint32_t tx;
    uint32_t mask;
} _uart_map_t;

static const _uart_map_t _valid_uart_map[] = {
    {PTE1,  PTE0,  (volatile uint32_t*)UART1_BASE,   3U, (volatile uint32_t*)PORTE_BASE, 1U,  0U,  SIM_SCGC4_UART1_MASK},
    {PTE21, PTE20, (volatile uint32_t*)UART0_BASE,   4U, (volatile uint32_t*)PORTE_BASE, 21U, 20U, SIM_SCGC4_UART0_MASK},
    {PTE23, PTE22, (volatile uint32_t*)UART2_BASE,   4U, (volatile uint32_t*)PORTE_BASE, 23U, 22U, SIM_SCGC4_UART2_MASK},
    {PTA1,  PTA2,  (volatile uint32_t*)UART0_BASE,   2U, (volatile uint32_t*)PORTA_BASE, 1U,  2U,  SIM_SCGC4_UART0_MASK},
    {PTA15, PTA14, (volatile uint32_t*)UART0_BASE,   3U, (volatile uint32_t*)PORTA_BASE, 15U, 14U, SIM_SCGC4_UART0_MASK},
    {PTA18, PTA19, (volatile uint32_t*)UART1_BASE,   3U, (volatile uint32_t*)PORTA_BASE, 18U, 19U, SIM_SCGC4_UART1_MASK},
    {PTB16, PTB17, (volatile uint32_t*)UART0_BASE,   3U, (volatile uint32_t*)PORTA_BASE, 16U, 17U, SIM_SCGC4_UART0_MASK},
    {PTC3,  PTC4,  (volatile uint32_t*)UART1_BASE,   3U, (volatile uint32_t*)PORTC_BASE, 3U,  4U,  SIM_SCGC4_UART1_MASK},
    {PTD2,  PTD3,  (volatile uint32_t*)UART2_BASE,   2U, (volatile uint32_t*)PORTD_BASE, 2U,  3U,  SIM_SCGC4_UART2_MASK},
    {PTD4,  PTD5,  (volatile uint32_t*)UART2_BASE,   2U, (volatile uint32_t*)PORTD_BASE, 4U,  5U,  SIM_SCGC4_UART2_MASK},
    {PTD6,  PTD7,  (volatile uint32_t*)UART0_BASE,   0U, (volatile uint32_t*)PORTD_BASE, 6U,  7U,  SIM_SCGC4_UART0_MASK},
    {NC,    NC,    (volatile uint32_t*)NULL,         NC, (volatile uint32_t*)NULL,       NC,  NC,                    NC},
};

#define VALID_UART_MAP_SIZE (sizeof(_valid_uart_map) / sizeof(_uart_map_t))

int8_t Uart_init(Uart_t* uart, Uart_conf_t* params) {
    int8_t ret = -1;
    PORT_Type* port;
    uint16_t uart_sbr;
    uint8_t idx;
    uint32_t mux;
    uint32_t pin;
    for(idx = 0; idx < (VALID_UART_MAP_SIZE - 1); idx++)
    {
        if((_valid_uart_map[idx].rx_name == params->rx) &&
-               (_valid_uart_map[idx].tx_name == params->tx))
        {
            uart = (UART_Type*)_valid_uart_map[idx].uart_register;
            register32_set_bits(&SIM->SCGC4, _valid_uart_map[idx].mask);
            mux = _valid_uart_map[idx].mux;
            if(uart == (UART_Type*)UART0_BASE)
            {
                /* Configure FLL/PLL Source. See p. 370. Only for UART0. */
                register32_clear_then_set_bits(&SIM->SOPT2,
                                               SIM_SOPT2_UART0SRC_MASK,
                                               SIM_SOPT2_UART0SRC(1));
                uart_sbr = (SystemCoreClock / UART0_OSR) / params->baud_rate;
                register8_clear_then_set_bits(&uart->C4, UARTLP_C4_OSR_MASK,
                                              UARTLP_C4_OSR(SystemCoreClock-1));
            } else {
                uart_sbr = SystemCoreClock / 16U / params->baud_rate;
            }

            if(uart_sbr > UART_SBR_MAX) {
                /* Desired baud rate cannot be attained. Uart handle not valid
                anymore. */
                uart = NULL;
                ret = -1;
                break;
            }

            port = (PORT_Type*)_valid_uart_map[idx].port_register;
            /* Select appropriate mux value to enable UART on pins */
            pin = _valid_uart_map[idx].tx;
            register32_clear_then_set_bits(&port->PCR[pin],
                                           PORT_PCR_MUX_MASK,PORT_PCR_MUX(mux));
            pin = _valid_uart_map[idx].rx;
            register32_clear_then_set_bits(&port->PCR[pin],
                                           PORT_PCR_MUX_MASK,PORT_PCR_MUX(mux));

            /* Register cleanup. */
            register8_write(&uart->C2, 0U);
            register8_write(&uart->C1, 0U);
            register8_write(&uart->C3, 0U);
            register8_write(&uart->S2, 0U);

            /* Write Baud Rate registers. */
            register8_clear_then_set_bits(&uart->BDH, UART_BDH_SBR_MASK,
                                         (uart_sbr >> 8U) & UART_BDH_SBR_MASK);
            register8_clear_then_set_bits(&uart->BDL, UART_BDL_SBR_MASK,
                                          uart_sbr & UART_BDL_SBR_MASK);

            /* Re-enable Tx and Rx, and allow interrupts for
               Receive Data Register Full Flag. See p. 728. */
            register8_set_bits(&uart->C2, UART_C2_RE_MASK | UART_C2_TE_MASK |
                               UART_C2_RIE_MASK);
            ret = 0;
        }
    }
    return ret;
}

int8_t Uart_get_default_conf(Uart_conf_t* conf) {
    int8_t ret = -1;
    if(conf) {
        memcpy(conf, &_uart_defualt_conf, sizeof(Uart_conf_t));
        ret = 0;
    }
    return ret;
}

int8_t Uart_write(Uart_t* uart, uint8_t byte) {
    int8_t ret = -1;
    if(uart) {
        register8_write(&uart->D, byte);
        ret = 0;
    }
    return ret;
}

uint8_t Uart_read(Uart_t* uart) {
    uint8_t byte = 0;
    if(uart) {
        byte = register8_read(&uart->D);
    }
    return byte;
}
