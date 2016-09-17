#ifndef UART_H
#define UART_H
#include <stdint.h>
#include "MKL25Z4.h"
#include "system_MKL25Z4.h"
#include "pins.h"

#define UART0_OSR 16U
#define UART_SBR_MAX 8191U
#define UART_STRING_SIZE_MAX 255U

typedef enum Stop_Bit_e {Stop_1bit_e=0U, Stop_2bit_e=1U} Stop_Bit_t;
typedef enum Bit_Mode_e {Mode_8bits_e, Mode_9bits_e} Bit_Mode_t;
typedef enum Loop_Mode_e {Mode_Normal_e=0U, Mode_Loop_e=1U} Loop_Mode_t;

typedef enum Parity_Mode_e {
    Parity_Disabled_e=0U, Parity_Enabled_e=1U
} Parity_Mode_t;

typedef enum Parity_Type_e {Parity_Even_e=0U, Parity_Odd_e=1U} Parity_Type_t;

typedef enum Buad_Rate_e {
    Baud_300_e=300U, Baud_600_e=600U, Baud_1200_e=1200U,
    Baud_2400_e=2400U, Baud_4800_e=4800U, Baud_9600_e=9600U,
    Baud_14400_e=14400U, Baud_19200_e=19200U, Baud_28800_e=28800U,
    Baud_38400_e=38400U, Baud_57600=57600U, Baud_115200_e=115200U,
    Baud_Default_e=Baud_9600_e
} Baud_Rate_t;

typedef UART_Type Uart_t;

typedef struct Uart_conf_s {
    Bit_Mode_t bit_mode;
    Stop_Bit_t stop_bits;
    Baud_Rate_t baud_rate;
    Parity_Mode_t parity_mode;
    Parity_Type_t parity_type;
    uint32_t tx;
    uint32_t rx;
} Uart_conf_t;

int8_t Uart_init(Uart_t* uart, Uart_conf_t* params);

int8_t Uart_putc(Uart_t* uart, char c);

int8_t Uart_puts(Uart_t* uart, char* msg);

char Uart_getc(Uart_t* uart);

int8_t Uart_gets(Uart_t* uart, char* msg);

int8_t Uart_write(Uart_t* uart, uint8_t byte);

int8_t Uart_write_bytes(Uart_t*, uint8_t* bytes, uint32_t size);

uint8_t Uart_read(Uart_t* uart);

int8_t Uart_read_bytes(Uart_t* uart, uint8_t* bytes, uint32_t size);

int8_t Uart_get_default_conf(Uart_conf_t* conf);

#endif // UART_H
