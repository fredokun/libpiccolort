CC=gcc
FLAGS=-Wall -I\include
NAME=pithread
LIB=lib
INCLUDE=include
SRC=src
HEADERS=runtime.h pithread.h
OBJ=runtime.o main.o

all : $(BIN)/$(NAME)

$(LIB)/%.o: $(SRC)/%.c $(DEPS)
	mkdir -p -v lib
	$(CC) -c -o $@ $< $(FLAGS)

$(BIN)/$(NAME): $(LIB)/$(OBJ)
	$(CC) -o $@ $^ $(FLAGS)

clean:
	rm -f bin/* lib/*
