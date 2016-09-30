#include "system_MKL25Z4.h"
#include "MKL25Z4.h"
#include "led.h"
#include "uart.h"
#include "log.h"
#include "delay.h"

#define APP_MAIN_ID (Log_ID_t)0U

int main(void) {
    Log_init();
    Log_set_level(APP_MAIN_ID, Log_Debug_e);
    LED_init(LED_GREEN); 
    while(1) {
        LED_toggle(LED_GREEN);
        Log(APP_MAIN_ID, Log_Debug_e, "This is a debug message");
        delay_ms(2000);
    }
    return 0;
}
