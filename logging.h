/**
 * Logging facilities.
 * @author Marcio Ribeiro (mmr)
 * @created 21/08/2005
 * @version $Id: logging.h,v 1.6 2006/01/13 00:35:29 mmr Exp $
 */

/* Types / Enums */
enum LOG_LEVELS {ERROR = 1, NOTICE = 2, WARN = 4, DEBUG = 8};

/* Constants */
#define LOG_FILE    "/tmp/service.log"
#define LOG_LEVEL   (ERROR|NOTICE|WARN|DEBUG)

/* Global vars */
FILE* logStream;

/* Prototypes */
void startLogging();
void stopLogging();
void logMessage(const int, const char*);
char* getLevelName(const int);