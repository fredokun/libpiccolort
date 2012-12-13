#############################
# Authors: Maxence WO       #
#############################

CC=gcc
FLAGS=-Wall -std=c99 -lpthread -I\include
NAME=pithread
BIN=bin
LIB=lib
INCLUDE=include
SRC=src
TESTS=tests
HEADERS=runtime.h pithread.h error.h definitions.h entry.h errors.h queue.h
OBJ=$(LIB)/main.o $(LIB)/entry.o $(LIB)/runtime.o $(LIB)/error.o $(LIB)/runtime_test.o $(LIB)/queue_test.o $(LIB)/queue.o

all : init
	mkdir -p $(LIB)
	mkdir -p $(TESTS)
	mkdir -p $(BIN)

init : $(BIN)/$(NAME)

$(LIB)/%.o: $(SRC)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(FLAGS)

$(LIB)/%.o: $(TESTS)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(FLAGS)

$(BIN)/$(NAME): $(OBJ)
	$(CC) -o $@ $^ $(FLAGS)

clean:
	rm -f bin/* lib/*

