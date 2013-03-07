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
<<<<<<< HEAD
OBJ=$(LIB)/pi_thread.o $(LIB)/commit.o $(LIB)/channel.o $(LIB)/scheduler.o $(LIB)/value.o $(LIB)/atomic.o $(LIB)/queue.o $(LIB)/concurrent.o $(LIB)/runtime.o $(LIB)/error.o $(LIB)/runtime_test.o $(LIB)/commit_test.o $(LIB)/pithread_test.o $(LIB)/queue_test.o $(LIB)/channel_test.o $(LIB)/atomic_test.o $(LIB)/run.o $(LIB)/knownset.o $(LIB)/tools.o
=======
SRCFILES=$(wildcard $(SRC)/*)
SRCFILES=$(wildcard $(SRC)/*.c)
TARG1=$(subst .c,.o, $(SRCFILES))
TESTSFILES=$(wildcard $(TESTS)/*.c)
TARG2=$(subst .c,.o, $(TESTSFILES))
OBJ=$(subst $(SRC), $(LIB), $(TARG1)) $(subst $(TESTS), $(LIB), $(TARG2))
>>>>>>> dev

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

