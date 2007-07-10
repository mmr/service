/**
 * Reconnect command.
 * @author Marcio Ribeiro (mmr)
 * @created 21/08/2005
 * @version $Id: reconnectCommand.c,v 1.12 2005/08/21 21:52:10 mmr Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "common.h"
#include "logging.h"

/* Constants */
#define LOCK_FILE   "/var/www/htdocs/b1n/service/reconnect.lock"
#define TEST_ADDR   "200.152.202.10"
#define TEST_PORT   80

#define SECONDS_TO_TIMEOUT      5
#define SECONDS_BETWEEN_CHECKS  10

/* Global vars */
volatile sig_atomic_t connected = FALSE;

/**
 * Called by when SIGALRM is caught (ie. timeout).
 * @param s signal issued.
 */
void timeOutHandler(int s) {
    connected = FALSE;
    signal(s, timeOutHandler);
}

/**
 * Check if is already connected (tries to create a simple HTTP connection).
 * @return TRUE if is already connected, FALSE otherwise.
 */
int isConnected() {
    int sock;
    struct sockaddr_in server;
    connected = FALSE;

    logMessage(DEBUG, "Reconnect: Checking if is connected.");

    if (!(sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))) {
        logMessage(ERROR, "Reconnect: could not create socket");
        exit(-1);
    }

    /* Setting server data */
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port   = htons(TEST_PORT);
    server.sin_addr.s_addr = inet_addr(TEST_ADDR);

    /* Attempting to Connect */
    /* When not connected, connect takes too long to timeout */
    /* That is why we need a timer to make this short */
    alarm(SECONDS_TO_TIMEOUT);
    if (connect(sock, (struct sockaddr *) &server, sizeof(server)) == 0) {
        connected = TRUE;
    }
    alarm(0);

    /* Closing socket */
    close(sock);

    return connected;
}

/**
 * Kill current PPP process (if any).
 * @return 0 if was able to kill PPP, non-0 otherwise.
 */
int killPPP() {
    return system("/usr/bin/pkill ppp");
}

/**
 * Start a new PPP connection.
 * @return 0 if was able to start PPP, non-0 otherwise.
 */
int startPPP() {
    return system("/usr/sbin/ppp -nat -ddial speedy");
}

/**
 * Check if is locked.
 * @return TRUE if is locked, FALSE otherwise.
 */
int isLocked() {
    struct stat buf; 
    return stat(LOCK_FILE, &buf) == 0;
}

/**
 * Create lock for reconnection.
 */
void createLock() {
    FILE* lockStream;

    if ((lockStream = fopen(LOCK_FILE, "w")) == NULL) {
        logMessage(ERROR, "Could not create Lock!");
        return;
    }
    fprintf(lockStream, "Reconnecting... Hold on!\n");
    fclose(lockStream);
}

/**
 * Destroy reconnection lock.
 */
void destroyLock() {
    unlink(LOCK_FILE);
}

/**
 * Restart PPP connection.
 */
void reconnect() {
    /* Killing PPP */
    killPPP();
    logMessage(NOTICE, "Reconnect: Sent TERM signal to PPP.");

    /* Restarting PPP */
    if (startPPP() == 0) {
        logMessage(NOTICE, "Reconnect: Starting PPP.");
    }

    /* Check if did connect (or lock was forced to destruction) */
    for (EVER) {
        if (isConnected()) {
            logMessage(NOTICE, "Reconnect: HOORAY, Connected!");
            break;
        }
        if (!isLocked()) {
            logMessage(ERROR, "Reconnect: Lock was removed! Aborting.");
            break;
        }
        sleep(SECONDS_BETWEEN_CHECKS);
    }
}

/** 
 * Check for connectivity, if is not already connected, connect.
 */
void reconnectCommand() {
    /* Set alarm handler */
    signal(SIGALRM, timeOutHandler);

    if (isLocked()) {
        logMessage(WARN, "Reconnect: Locked, please wait.");
    } else {
        /* Create lock */
        createLock();

        if (isConnected()) {
            logMessage(WARN, "Reconnect: Already connected!");
        } else {
            logMessage(WARN, "Reconnect: Not connected!");
            reconnect();
        }

        /* Destroy lock */
        destroyLock();
    }
}

/**
 * Force lock destruction.
 */
void forceLockCommand() {
    destroyLock();
}
