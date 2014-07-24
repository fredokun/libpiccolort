# Makefile PiThread
#
# This project is released under MIT License.
#

# tools and options
CC        = gcc
LCC       = ar -rs
CFLAGS    = -g -Wall -std=c99 -I\include
OFLAGS    = -lpthread
OFLAGS2   = -L\lib -lpiccolort -lpthread

# targets
LIBTARG   = libpiccolort.a
TESTSTARG = picc_tests

# directories
LIB       = lib
INC       = include
SRC       = src
OBJ       = obj
BIN       = bin
TESTS     = tests

LIBFILES  = $(wildcard $(SRC)/*.c)
LIBOBJS   = $(subst .c,.o, $(subst $(SRC),$(OBJ), $(LIBFILES)))
TESTFILES = $(wildcard $(TESTS)/*.c)
TESTOBJS  = $(subst .c,.o, $(subst $(TESTS),$(OBJ), $(TESTFILES)))

all: init lib tests

init:
	mkdir -p $(OBJ) $(LIB) $(BIN)

lib: init $(LIB)/$(LIBTARG)

tests: init $(BIN)/$(TESTSTARG)

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(LIB)/$(LIBTARG): $(LIBOBJS)
	$(LCC) $@ $^

$(OBJ)/%.o: $(TESTS)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(BIN)/$(TESTSTARG): $(TESTOBJS)
	$(CC) -o $@ $^ $(OFLAGS2)

clean:
	rm -f $(OBJ)/*

cleanall: clean
	rm -rf $(OBJ) $(LIB) $(BIN)

