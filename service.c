/**
 * Service: Get/Process commands from clients throught UDP.
 * @author Marcio Ribeiro (mmr)
 * @created 20/08/2005
 * @version $Id: service.c,v 1.7 2006/01/13 00:35:29 mmr Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "common.h"
#include "logging.h"
#include "command.h"
#include "service.h"

int main(int argc, char** argv) {
    /* Check if we got args */
    if (argc > 1) {
        usage(argv[0]);
        terminate(0);
    }

    /* Check if we got enough perms */
    //checkIfWeAreRoot();

    /* Trap signals */
    trapSignals();

    /* Check PID file */
    checkPidFile();

    /* Detach (Daemonize) */
    if (fork() == 0) {
        cleanUp = TRUE;

        /* Start logging right away! */
        startLogging();
        logMessage(NOTICE, "Starting");

        /* Write PID */
        writePidFile(getpid());

        /* Load Commands */
        loadCommands();

        /* Wait for commands */
        waitForCommands();

        /* Bye bye */
        terminate(0);
    }

    return 0;
}

/**
 * You have to be root (ie. UID or EUID equal 0) to run this program.
 */
void checkIfWeAreRoot() {
    if (getuid() != 0 && geteuid() != 0) {
        fprintf(stderr, "Poor mortal.\n");
        terminate(-1);
    }
}

/**
 * Trap signals.
 */
void trapSignals() {
    signal(SIGHUP,  SIG_IGN);
    signal(SIGINT,  (void *) terminate);
    signal(SIGTERM, (void *) terminate);
    signal(SIGSTOP, SIG_IGN);
}

/**
 * Instructs how to use this program.
 * @param progName name of the program.
 */
void usage(const char* progName) {
    fprintf(stderr, "%s v%s\nUsage: %s\n", PROGNAME, VERSION, progName);
}

/**
 * Check if PID file already exists.
 * If so, the program may be already running, abort.
 */
void checkPidFile(const int pid) {
    struct stat aux;
    if(stat(PID_FILE, &aux) == 0) {
        fprintf(stderr, "Program already running.\n");
        terminate(2);
    }
}

/**
 * Write PID to file.
 * @param pid PID number of child (daemon) process.
 */
void writePidFile(const int pid) {
    FILE* pidStream;

    if((pidStream = fopen(PID_FILE, "w")) == NULL) {
        fprintf(stderr, "Could not open '%s'.", PID_FILE);
        terminate(3);
    }
    fprintf(pidStream, "%d", pid);
    fclose(pidStream);
}

/**
 * Waits for commands from clients.
 * TODO use UNIX Sockets.
 */
void waitForCommands() {
    int sock;
    char buf[BUFSIZ];
    struct sockaddr_in server;
    struct sockaddr_in client;
    socklen_t len;

    /* Creating UDP socket */
    if(!(sock = socket(AF_INET, SOCK_DGRAM, 0))) {
        bail("Could not create Socket");
    }

    /* Setting server data */
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    //server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_addr.s_addr = inet_addr(ADDR);

    /* Binding to port */
    if (bind(sock, (struct sockaddr *) &server, sizeof(server)) != 0) {
        snprintf(buf, sizeof(buf), "Could not bind to port %d", PORT);
        bail(buf);
    }
    logMessage(DEBUG, "Waiting for clients...");

    /* Processing commands */
    for (EVER) {
        bzero(&buf, sizeof(buf));
        if(recvfrom(sock, buf, sizeof(buf), 0,
                (struct sockaddr *) &client, &len) == -1)
        {
            bail("Could not get data from client");
        }

        runCommand(buf);
    }

    /* Closing socket */
    close(sock);
}

/**
 * Terminate program execution with an error message.
 * @param msg message logged before termination.
 */
void bail(const char* msg) {
    logMessage(ERROR, msg);
    terminate(1);
}

/**
 * Clean up before program execution end.
 * @param ret return value to shell.
 */
void terminate(const int ret) {
    if (cleanUp) {
        /* Stop logging */
        logMessage(NOTICE, "Exiting");
        stopLogging();

        /* Unload commands */
        unloadCommands();

        /* Deleting PID file */
        unlink(PID_FILE);
    }

    /* Cya */
    exit(ret);
}
