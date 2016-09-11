#include "uart.h"
#include "reg.h"
#include <stddef.h>
#include <stdio.h>

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
    int8_t idx;
    uint32_t mask;
    uint32_t pin;
    uint32_t mux;
    PORT_Type* _port;
    if(uart && params) {
        for(idx = 0; idx < (VALID_UART_MAP_SIZE - 1); idx++)
        {
            /* If the desired Rx and Tx pins can be muxed to a UART module,
            then the desired UART configuration is valid. */
            if((_valid_uart_map[idx].rx_name == params->rx) &&
               (_valid_uart_map[idx].tx_name == params->tx)) {

                uart = (UART_Type*)_valid_uart_map[idx].uart_register;

                /* Enable clock */
                mask = _valid_uart_map[idx].mask;
                register32_set_bits(&SIM->SCGC4, mask);

                if (_valid_uart_map[idx].uart_register == (uint32_t*)UART0_BASE)
                {
                    /* Configure clock source for UART0 */
                    register32_clear_bits(&SIM->SOPT2, SIM_SOPT2_UART0SRC_MASK);
                    register32_set_bits(&SIM->SOPT2, SIM_SOPT2_UART0SRC(0x01U));
                    // register32_clear_bits(&SIM->SOPT2, SIM_SOPT2_PLLFLLSEL_MASK);
                }

                _port = (PORT_Type*)_valid_uart_map[idx].port_register;

                /* Configure mux. Chap. 11, p. 183. */
                mux = _valid_uart_map[idx].mux;
                /* Configure Rx mux signal. */
                pin = _valid_uart_map[idx].rx;
                register32_clear_bits(&_port->PCR[pin], PORT_PCR_MUX_MASK);
                register32_set_bits(&_port->PCR[pin], PORT_PCR_MUX(mux));
                /* Configure Tx mux signal. */
                pin = _valid_uart_map[idx].tx;
                register32_clear_bits(&_port->PCR[pin], PORT_PCR_MUX_MASK);
                register32_set_bits(&_port->PCR[pin], PORT_PCR_MUX(mux));

                /* Disable Tx and Rx before configuration. Set registers
                to the default status. */
                register8_write(&uart->C1, 0x00U);
                register8_write(&uart->C2, 0x00U);
                register8_write(&uart->C3, 0x00U);
                register8_write(&uart->S2, 0x00U);

                /* Enable transmitter and receiver. */
                //register8_set_bits(&uart->C2,
                //                   UART_C2_TE_MASK | UART_C2_RE_MASK);

                #define OVER_SAMPLE 16U
                uint16_t divisor = (SystemCoreClock / 2 / OVER_SAMPLE) / Baud_9600_e;
                register8_clear_bits(&uart->C4, UART0_C4_OSR_MASK);
                register8_set_bits(&uart->C4, OVER_SAMPLE - 1U);
                /* Set baudrate */
                register8_clear_bits(&uart->BDH, UART0_BDH_SBR_MASK);
                register8_set_bits(&uart->BDH, (divisor >> 8U) &
                                   UARTLP_BDH_SBR_MASK);
                register8_write(&uart->BDL, divisor & UART0_BDL_SBR_MASK);

                /* Configure desired Baud Rate */
                //Uart_set_baud_rate(uart, params->baud_rate);
                //Uart_set_parity_type(uart, params->parity_type);
                //Uart_set_parity_mode(uart, params->parity_mode);
                ret = 0;
                break;
            } else {
                // This is not the desired UART, go to the next one.
            }
        }
    } else {
        /* Invalid parameters provided. */
    }
    return ret;
}

int8_t Uart_set_baud_rate(Uart_t* uart, Baud_Rate_t br) {
    uint32_t tmp;
    uint32_t busclk;
    uint32_t regval;
    int8_t ret = -1;
    if((uart != (UART_Type*)UART0_BASE) && (br > 0U)) {
        tmp = register32_read(&SIM->CLKDIV1);
        tmp = (tmp & SIM_CLKDIV1_OUTDIV4_MASK) >> SIM_CLKDIV1_OUTDIV4_SHIFT;
        busclk = SystemCoreClock / (tmp + 1U);
        tmp = busclk / (16U * br);
        /* BCR register is 13 bit wide. If the result is higher, then the
        desired baud rate cannot be configured. */
        if((tmp <= 0x1FFFU) && (tmp >= 1U)) {
            regval = (tmp >> 8U) & UART_BDH_SBR_MASK;
            register8_write(&uart->BDH, regval);
            regval = tmp & UART_BDL_SBR_MASK;
            register8_write(&uart->BDL, regval);
            ret = 0;
        }
    } else {

    }
    return ret;
}

int8_t Uart_set_stop_bit(Uart_t *uart, Stop_Bit_t stopbit) {
    int8_t ret = -1;
    if(uart) {
        if(stopbit) {
            /*  2-bit Stop */
            register8_set_bits(&uart->BDH,  UART_BDH_SBNS_MASK);
        } else {
            /* 1-bit Stop */
            register8_clear_bits(&uart->BDH, UART_BDH_SBNS_MASK);
        }
    }
    return ret;
}

int8_t Uart_bit_mode(Uart_t* uart, Bit_Mode_t bitmode) {
    int8_t ret = -1;
    if(uart) {
        if(bitmode) {
            /*  9-bit Characters */
            register8_set_bits(&uart->C1, UART_C1_M_MASK);
        } else {
            /* 8-bit Characters */
            register8_clear_bits(&uart->C1, UART_C1_M_MASK);
        }
    }
    return ret;
}

int8_t Uart_set_parity_type(Uart_t* uart, Parity_Type_t parity) {
    int8_t ret = -1;
    if(uart) {
        if(parity) {
            register8_set_bits(&uart->C1, UART_C1_PT_MASK);
        } else {
            register8_clear_bits(&uart->C1, UART_C1_PT_MASK);
        }
        ret = 0;
    }
    return ret;
}

int8_t Uart_set_parity_mode(Uart_t* uart, Parity_Mode_t mode) {
    int8_t ret = -1;
    if(uart) {
        if(mode) {
            register8_set_bits(&uart->C1, UART_C1_PE_MASK);
        } else {
            register8_clear_bits(&uart->C1, UART_C1_PE_MASK);
        }
        ret = 0;
    }
    return ret;
}

int8_t Uart_get_default_conf(Uart_conf_t* conf) {
    int8_t ret = -1;
    if(conf) {
        conf->bit_mode = Mode_8bits_e;
        conf->stop_bits = Stop_1bit_e;
        conf->baud_rate = Baud_115200_e;
        conf->parity_mode = Parity_Even_e;
        conf->parity_type = Parity_Disabled_e;
        conf->tx = PTA2;
        conf->rx = PTA1;
    }
    return ret;
}

int8_t Uart_write_byte(Uart_t* uart, uint8_t byte) {
    int8_t ret = -1;
    if(uart) {
        register8_write(&uart->D, byte);
        ret = 0;
    }
    return ret;
}

uint8_t Uart_read_byte(Uart_t* uart) {
    uint8_t byte = 0;
    if(uart) {
        byte = register8_read(&uart->D);
    }
    return byte;
}
