# Makefile PiThread
#
# This project is released under MIT License.
#
# @author Maxence WO
# @author Mickaël MENU
# @author Dany SIRIPHOL

CC=gcc
CFLAGS=-g -Wall -std=c99 -I\include -I\tests
OFLAGS= -lpthread
NAME=run_tests
BIN=bin
LIB=lib
INCLUDE=include
SRC=src
TESTS=tests

SRCFILES=$(wildcard $(SRC)/*)
OBJ=$(patsubst %.c, %.o, $(SRCFILES))

test:
	echo $(OBJ)

all : clean init $(BIN)/$(NAME)

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

