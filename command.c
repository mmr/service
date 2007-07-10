/**
 * Commands.
 * @author Marcio Ribeiro (mmr)
 * @created 21/08/2005
 * @version $Id: command.c,v 1.7 2005/08/21 19:02:05 mmr Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "logging.h"
#include "command.h"

/**
 * Load commands into data structure.
 */
void loadCommands() {
    Command* curCommand;

    logMessage(DEBUG, "Loading commands...");

    curCommand = commands = (Command *) malloc(sizeof(Command));
    curCommand = addCommand(curCommand, "reconnect", reconnectCommand);
    curCommand = addCommand(curCommand, "forceLock", forceLockCommand);
}

/**
 * Add a new commnad.
 * @param curCommand current command.
 * @param name name of the command.
 * @param commandPointer pointer to the function of the command.
 * @return pointer to next command.
 */
Command* addCommand(Command* curCommand, const char* name,
        const void* commandPointer)
{
    char aux[BUFSIZ];

    snprintf(curCommand->name, sizeof(curCommand->name), name);
    curCommand->run  = commandPointer;
    curCommand->next = (Command *) malloc(sizeof(Command));
    curCommand = curCommand->next;

    snprintf(aux, sizeof(aux), "Added %s command", name);
    logMessage(DEBUG, aux);

    return curCommand;
}

/**
 * Looks for a command in the commands data and run it.
 * @param name the name of the command.
 */
void runCommand(const char* name) {
    char aux[BUFSIZ];
    int foundCommand = FALSE;
    Command* command;

    command = commands;
    while (command != NULL) {
        if (strncmp(name, command->name, sizeof(command->name)) == 0) {
            snprintf(aux, sizeof(aux), "Running %s", name);
            logMessage(NOTICE, aux);
            command->run();
            foundCommand = TRUE;
            break;
        }
        command = command->next;
    }

    if (!foundCommand) {
        snprintf(aux, sizeof(aux), "Unknown command '%s'", name);
        logMessage(WARN, aux);
    }
}

/**
 * Frees the memory used by the commands data structure.
 */
void unloadCommands() {
    Command* command;
    Command* next;

    command = commands;
    while (command->next != NULL) {
        next = command->next;
        free(command);
        command = next;
    }
}
