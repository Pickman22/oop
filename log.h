#ifndef LOG_H
#define LOG_H

typedef uint8_t Log_ID_t;

typedef enum Log_Level {
    Log_Debug_e, Log_Info_e, Log_Warn_e, Log_Error_e
} Log_Level_e;

typedef struct Log_State_s {
    Log_Module_t id;
    Log_Level_e lvl;
} Log_State_t;

#define LOGGER_SIZE 8U

void Log_init();

void Log(Log_Module_t id, Log_Level_e lvl, char* msg);

void Log_set_level(Log_Module_t id, Log_Level_e lvl);

#endif // LOG_H
