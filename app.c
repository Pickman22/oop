#include "system_MKL25Z4.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "led.h"
#include "fsl_port.h"
#include "uart.h"
#include "log.h"

#define APP_MAIN_ID (Log_ID_t)0U

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
    Log_init();
    Log_set_level(APP_MAIN_ID, Log_Error_e);
    //Uart_conf_t conf = Uart_get_default_conf();
    //Uart_t* uart = Uart_init(conf);
    LED_init(LED_green_e);
    while(1) {
        //UART0->D = 'a';
        delay();
        Log(APP_MAIN_ID, Log_Debug_e, "This is a test!");
        Log(APP_MAIN_ID, Log_Warn_e, "This is a warning!");
        Log(APP_MAIN_ID, Log_Error_e, "This is an error!!!!");
        Log(APP_MAIN_ID, Log_Info_e, "This is an info msg!!");
        //Uart_puts(uart, "This ia a test!\n\r");
        LED_set(LED_green_e);
        delay();
        LED_clear(LED_green_e);
        delay();
    }
}
