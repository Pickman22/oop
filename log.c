#include "log.h"
#include "uart.h"
#include <assert.h>
#include <stddef.h>

static Uart_t* _uart = NULL;
static Log_Level_e _module_levels[LOGGER_SIZE] = {Log_Off_e};

void _logger_write(char* msg) {
    /* Write the string to the interface. */
    if(_uart && msg) {
        Uart_puts(_uart, msg);
    }
}

void Log_init() {
    /* Initialize the logging interface. */
    Uart_conf_t params = Uart_get_default_conf();
    _uart = Uart_init(params);
}

void Log_set_level(Log_ID_t id, Log_Level_e lvl) {
    if (id < LOGGER_SIZE) {
        _module_levels[id] = lvl;
    }
}

void Log(Log_ID_t id, Log_Level_e lvl, char* msg) {
    if((id < LOGGER_SIZE) && _uart && msg) {
        if ((lvl >= _module_levels[id]) && _module_levels[id] != Log_Off_e)
        {
            switch(lvl)
            {
                case Log_Debug_e:
                    _logger_write("[DEBUG]: ");
                    break;
                case Log_Info_e:
                    _logger_write("[INFO]: ");
                    break;
                case Log_Warn_e:
                    _logger_write("[WARN]: ");
                    break;
                case Log_Error_e:
                    _logger_write("[ERROR]: ");
                    break;
                default:
                    assert(0);
            }
            _logger_write(msg);
            _logger_write("\n\r");
        }
    }
}
