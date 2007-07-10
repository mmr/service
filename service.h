/**
 * Service: Get/Process commands from clients throught UDP.
 * @author Marcio Ribeiro (mmr)
 * @created 21/08/2005
 * @version $Id: service.h,v 1.10 2006/01/13 00:35:29 mmr Exp $
 */

/* Constants */
#define PID_FILE    "/tmp/service.pid"
#define ADDR        "127.0.0.1"
#define PORT        6666

/* Prototypes */
void bail(const char*);
void checkIfWeAreRoot();
void checkPidFile();
void terminate(const int);
void trapSignals();
void usage(const char*);
void waitForCommands();
void writePidFile(const int);

/* Global vars */
int cleanUp = FALSE;
