#ifndef LOG_H
#define LOG_H

#include <stdint.h>

typedef uint8_t Log_ID_t;

typedef enum Log_Level {
    Log_Debug_e, Log_Info_e, Log_Warn_e, Log_Error_e, Log_Off_e
} Log_Level_e;

#define LOGGER_SIZE 8U

void Log_init();

void Log(Log_ID_t id, Log_Level_e lvl, char* msg);

void Log_set_level(Log_ID_t id, Log_Level_e lvl);

#endif // LOG_H
