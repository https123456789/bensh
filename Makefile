CC = g++
SRC := $(wildcard src/*.cpp)
OFILES := $(SRC:cpp=o)
CFLAGS := -Wall -std=c++20
BIN ?= bin
MODE ?= Debug

$(BIN)/$(MODE)/bensh: $(OFILES)
	@mkdir -p $(BIN)/$(MODE)
	@echo "Linking..."
	@$(CC) -o $@ $(CFLAGS) $^

%.o: %.cpp
	@echo "Building $@..."
	@$(CC) -c -o $@ $(CFLAGS) $^

clean: $(BIN)/$(MODE)/bensh $(OFILES)
	@rm -v $^

run: $(BIN)/$(MODE)/bensh
	./$<
