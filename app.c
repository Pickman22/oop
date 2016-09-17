#include "system_MKL25Z4.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "led.h"
#include "fsl_port.h"
#include "uart.h"

void delay(void) {
    volatile uint32_t i = 300000U;
    while (i--)
    {
        __NOP();
    }
}

void init(void) {
    BOARD_BootClockRUN();
    CLOCK_EnableClock(kCLOCK_PortA);
    CLOCK_EnableClock(kCLOCK_PortB);
    CLOCK_EnableClock(kCLOCK_PortC);
    CLOCK_EnableClock(kCLOCK_PortD);
    CLOCK_EnableClock(kCLOCK_PortE);
}

void main(void) {
    init();
    Uart_conf_t conf = Uart_get_default_conf();
    Uart_t* uart = Uart_init(conf);
    LED_t* rled = LED_get_instance(LED_red_e);
    while(1) {
        UART0->D = 'a';
        delay();
        Uart_write(uart, 'z');
        LED_set(rled);
        delay();
        LED_clear(rled);
        delay();
    }
}
