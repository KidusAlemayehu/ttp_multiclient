#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include "logger.h"

void log_msg(const char *level, const char *msg, ...){
    va_list args;
    va_start(args, msg);

    time_t now = time(NULL);
    char *time_str = ctime(&now);
    time_str[strlen(time_str) - 1] = '\0';

    printf("[%s] %s - ", time_str, level);

    vprintf(msg, args);

    va_end(args);
    printf("\n");
}

void log_info(const char *msg, ...){
    va_list args;
    va_start(args, msg);
    log_msg("INFO", msg, args);
    va_end(args);
}

void log_error(const char *msg, ...){
    va_list args;
    va_start(args, msg);
    log_msg("ERROR", msg, args);
    va_end(args);
}