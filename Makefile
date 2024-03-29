CC = g++
BIN ?= bin
MODE ?= Debug
SRC := $(wildcard src/*.cpp)
HEADERS := $(wildcard src/*.hpp)
PREFIXED_SRC := $(addprefix build/,$(SRC))
OFILES := $(PREFIXED_SRC:cpp=o)
CFLAGS := -Wall -std=c++20

$(BIN)/$(MODE)/bensh: format lint $(OFILES)
	@mkdir -p $(BIN)/$(MODE)
	@echo "Linking..."
	@$(CC) -o $@ $(CFLAGS) $(OFILES)

build/%.o: %.cpp
	@mkdir -p $(shell dirname $@)
	@echo "Building $@..."
	@$(CC) -c -o $@ $(CFLAGS) $^

.PHONY: format
format: $(SRC) $(HEADERS)
	@echo "Formatting..."
	@clang-format -i $(SRC) $(HEADERS) --Werror

.PHONY: format-check
format-check: $(SRC) $(HEADERS)
	@echo "Checking Formatting..."
	@clang-format --dry-run $(SRC) $(HEADERS) --Werror

.PHONY: lint
lint: $(SRC) $(HEADERS)
	@echo "Linting..."
	@cpplint $(SRC) $(HEADERS)

.PHONY: clean
clean:
	@rm -r build bin

.PHONY: run
run: $(BIN)/$(MODE)/bensh
	./$<
