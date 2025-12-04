#include "logging.h"
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
    if (!slash) return;

    *slash = 0;
    mkdir(path, 0775);
}

bool logger_init(const char *filepath, const LogLevel level, const bool use_stdout) {

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

