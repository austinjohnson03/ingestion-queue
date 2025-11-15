#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

typedef enum {
    LOG_DEBUG = 0,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR
} LogLevel;

typedef struct Logger {
    FILE *file;
    LogLevel level;
    bool use_stdout; 
    pthread_mutex_t lock;
} Logger;

extern Logger LOGGER;

void init_logger(Logger *logger, const char *filepath, LogLevel level, bool use_stdout);
void logger_close(Logger *logger);

void logger_log(Logger *logger, LogLevel level, const char *fmt, ...);

#define log_debug(fmt, ...) logger_log(LOG_DEBUG, fmt, ##__VA_ARGS__)
#define log_info(fmt, ...) logger_log(LOG_INFO, fmt, ##__VA_ARGS__)
#define log_warn(fmt, ...) logger_log(LOG_WARN, fmt, ##__VA_ARGS__)
#define log_error(fmt, ...) logger_log(LOG_ERROR, fmt, ##__VA_ARGS__)

#endif // LOGGING_H