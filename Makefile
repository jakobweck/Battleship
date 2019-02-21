CC		:= g++
C_FLAGS := -std=c++17 -g -Wall -Wextra

BIN		:= bin
SRC		:= src
INCLUDE	:= include
LIB		:= lib

LIBRARIES	:= -pthread

ifeq ($(OS),Windows_NT)
EXECUTABLE	:= battleship.exe
else
EXECUTABLE	:= battleship
endif

all: $(BIN)/$(EXECUTABLE) 

clean:
	$(RM) $(BIN)/$(EXECUTABLE)

run: all
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*
	$(CC) $(C_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)