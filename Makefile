# $Id: Makefile,v 1.4 2005/08/21 17:29:05 mmr Exp $

PROG=service
OBJS=service.o logging.o command.o reconnectCommand.o
CC=cc
DEFS=
CFLAGS=-O2 -Wall
INC=
LIB=

all: service

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(DEFS) $(OBJS) $(INC) $(LIB) -o $@

$(PROG).o: service.c
	$(CC) $(CFLAGS) $(DEFS) $(INC) -c $(PROG).c

distclean: clean
	rm -f *~

clean:
	rm -f $(OBJS) $(HDRS) $(PROG) $(PROG).core
