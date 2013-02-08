# Makefile PiThread
#
# This project is released under MIT License.
#
# @author Maxence WO
# @author Mickaël MENU
# @author Dany SIRIPHOL

CC=gcc
CFLAGS=-Wall -std=c99 -I\include
OFLAGS= -lpthread
NAME=run_tests
BIN=bin
LIB=lib
INCLUDE=include
SRC=src
TESTS=tests
HEADERS=runtime.h pithread.h error.h definitions.h entry.h errors.h queue.h set.h runtime_tests.h
OBJ=$(LIB)/pi_thread.o $(LIB)/commit.o $(LIB)/channel.o $(LIB)/scheduler.o $(LIB)/value.o $(LIB)/queue.o $(LIB)/sync.o $(LIB)/runtime.o $(LIB)/error.o $(LIB)/runtime_test.o $(LIB)/queue_test.o $(LIB)/set.o $(LIB)/set_functions.o $(LIB)/set_test.o $(LIB)/channel_test.o $(LIB)/main.o $(LIB)/tools.o

all : clean init $(BIN)/$(NAME)

test : $(BIN)/main

$(BIN)/main: $(LIB)/main.o
	$(CC) -o $@ $^ $(OFLAGS)

$(LIB)/main.o: $(SRC)/main.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

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

