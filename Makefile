# Makefile PiThread
#
# This project is released under MIT License.
#
# @author Maxence WO
# @author Mickaël MENU

CC=gcc
CFLAGS=-Wall -std=c99 -I\include
OFLAGS= -lpthread
NAME=pithread
BIN=bin
LIB=lib
INCLUDE=include
SRC=src
TESTS=tests
HEADERS=runtime.h pithread.h error.h definitions.h entry.h errors.h queue.h
OBJ=$(LIB)/pi_thread.o $(LIB)/commit.o $(LIB)/channel.o $(LIB)/scheduler.o $(LIB)/value.o $(LIB)/queue.o $(LIB)/sync.o $(LIB)/runtime.o $(LIB)/error.o $(LIB)/runtime_test.o $(LIB)/queue_test.o

all : clean init $(BIN)/$(NAME)

init :
	mkdir -p $(LIB)
	mkdir -p $(TESTS)
	mkdir -p $(BIN)

$(LIB)/%.o: $(SRC)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(LIB)/%.o: $(TESTS)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BIN)/$(NAME): $(OBJ)
	$(CC) -o $@ $^ $(OFLAGS)

clean:
	rm -f bin/* lib/*

