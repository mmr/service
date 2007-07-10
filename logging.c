/**
 * Logging facilities.
 * @author Marcio Ribeiro (mmr)
 * @created 21/08/2005
 * @version $Id: logging.c,v 1.6 2005/08/21 17:29:05 mmr Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#include "common.h"
#include "logging.h"

/**
 * Initialize logging.
 */
void startLogging() {
    if ((logStream = fopen(LOG_FILE, "a")) == NULL) {
        fprintf(stderr, "Could not open '%s'.", LOG_FILE);
        exit(3);
    }
}

/**
 * Stop logging.
 */
void stopLogging() {
    fclose(logStream);
}

/**
 * Log a message to the logging stream (usually a file).
 * @param level error level (see LOG_LEVELS enum).
 * @param msg message to be logged.
 */
void logMessage(const int level, const char* msg) {
    char aux[BUFSIZ];
    const time_t t = time(NULL);

    if ((LOG_LEVEL & level) != level) {
        return;
    }

    snprintf(aux, sizeof(aux), "%.24s %s\t%s\n", ctime(&t),
            getLevelName(level), msg);

    fprintf(logStream, "%s", aux);
    fflush(logStream);
}


char* getLevelName(const int level) {
    char* levelName;

    switch (level) {
    case ERROR:
        levelName   = "ERROR";
        break;
    case WARN:
        levelName   = "WARN";
        break;
    case NOTICE:
        levelName   = "NOTICE";
        break;
    case DEBUG:
        levelName   = "DEBUG";
        break;
    default:
        levelName   = "UNKNOWN";
    }

    return levelName;
}
