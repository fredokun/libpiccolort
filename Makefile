# Makefile PiThread
#
# This project is released under MIT License.
#
# @author Maxence WO
# @author Mickaël MENU
# @author Dany SIRIPHOL

CC=gcc
LCC=ar -rs
CFLAGS=-g -Wall -std=c99 -I\include -I\tests
OFLAGS= -lpthread
NAME=run_tests
LIB_NAME=pirt
FULL_LIB_NAME=lib$(LIB_NAME).a
BIN=bin
LIB=lib
INCLUDE=include
SRC=src
TESTS=tests

SRCFILES=$(wildcard $(SRC)/*.c)
TARG1=$(subst .c,.o, $(SRCFILES))
LIB_OBJ=$(subst $(SRC), $(LIB), $(TARG1))
TESTSFILES=$(wildcard $(TESTS)/*.c)
TARG2=$(subst .c,.o, $(TESTSFILES))
OBJ=$(LIB_OBJ) $(subst $(TESTS), $(LIB), $(TARG2))


all : clean init $(BIN)/$(NAME) $(LIB)/$(FULL_LIB_NAME)

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

$(LIB)/$(FULL_LIB_NAME): $(LIB_OBJ)
	$(LCC) $@ $^

clean:
	rm -f bin/* lib/*

