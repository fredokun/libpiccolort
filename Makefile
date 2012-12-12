#############################
# Authors: Maxence WO       #
#############################

CC=gcc
FLAGS=-Wall -lpthread -I\include
NAME=pithread
BIN=bin
LIB=lib
INCLUDE=include
SRC=src
HEADERS=runtime.h pithread.h error.h definitions.h entry.h
OBJ=$(LIB)/entry.o $(LIB)/runtime.o $(LIB)/main.o $(LIB)/error.o $(LIB)/runtimeTests.o

all : $(BIN)/$(NAME)

$(LIB)/%.o: $(SRC)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(FLAGS)

$(BIN)/$(NAME): $(OBJ)
	$(CC) -o $@ $^ $(FLAGS)

clean:
	rm -f bin/* lib/*
