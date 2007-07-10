/**
 * Commands.
 * @author Marcio Ribeiro (mmr)
 * @created 21/08/2005
 * @version $Id: command.h,v 1.6 2005/08/21 19:17:24 mmr Exp $
 */

/* TODO implement a better data structure (eg. Hash table, O(1)) */
typedef struct Command {
    char name[32];          /* Command name */
    void (* run)(void);     /* Function pointer to command execution */
    struct Command* next;   /* Next command in the LinkedList */
} Command;

/* Prototypes */
Command* addCommand(Command*, const char*, const void*);
void loadCommands();
void runCommand(const char*);
void unloadCommands();

void reconnectCommand();
void forceLockCommand();

/* Global vars */
Command* commands;
