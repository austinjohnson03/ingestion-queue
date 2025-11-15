#include "logger.h"
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#define MAX_PATH 256

Logger LOGGER;

static const char *level_strings[] = {
    "DEBUG",
    "INFO",
    "WARNING",
    "ERROR"
};

static void ensure_log_dir_exists(const char *filepath) {
    char path[MAX_PATH];
    strncpy(path, filepath, sizeof(path));
    path[sizeof(path) - 1] = 0;

    char *slash = strrchr(path, '/');
    if (!slash) return false;

    *slash = 0;
    mkdir(path, 0775);
}

bool logger_init(const char *filepath, LogLevel level, bool use_stdout) {

    ensure_log_dir_exists(filepath);

    LOGGER.file = fopen(filepath, "a");
    if (!LOGGER.file) {
        fprintf(stderr, "Logger failed to open file: %s\n", filepath);
        return false;
    }

    LOGGER.level = level;
    LOGGER.use_stdout = use_stdout;

    pthread_mutex_init(&LOGGER.lock, NULL);

    return true;
}

void logger_log(Logger *logger, LogLevel level, const char *fmt, ...) {
    if (!logger || !logger->file) return false;
    if (level < logger->level) return;

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    char timebuf[32];
    strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", t);

    fprintf(logger->file, "[%s] [%s] ", timebuf, level_strings[level]);

    va_list args;
    va_start(args, fmt);
    vprintf(logger->file, fmt, args);
    va_end(args);

    fprintf(logger->file, "\n");
    fflush(logger->file);

    if (logger->use_stdout) {
        printf("[%s] [%s] ", timebuf, level_strings[level]);
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        printf("\n");
    }
}

void logger_close(void) {
    if (LOGGER.file) {
        fclose(LOGGER.file);
        LOGGER.file = NULL;
    }

    pthread_mutex_destory(&LOGGER.lock);
}