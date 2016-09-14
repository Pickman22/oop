#include "log.h"
#include "uart.h"
#include <stdint.h>

static uint8_t _logger_is_init = 0U;

struct _log_state_s {
    Log_ID_t module_id;
    Log_Level_e module_lvl;
};

static struct _log_state_s _logger_state[LOGGER_SIZE] = {0};

void _logger_write(char* msg) {
    /* Write the string to the interface. */
}

void Log_init(Log_ID_t id, Log_Level_e lvl) {
    /* Initialize the logging interface. */
    _logger_is_init = 1U;
}

void Log_set_level(Log_ID_t id, Log_Level_e lvl) {
    if (id < LOGGER_SIZE) {
        _logger_state[id].level = lvl;
    }
}

void Log(Log_ID_t id, Log_Level_e lvl, char* msg) {
    if((id < LOGGER_SIZE) && _logger_is_init) {
        if (lvl >= _logger_state[id].level)
        {
            switch(_logger_state[id].level)
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
