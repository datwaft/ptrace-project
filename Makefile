# Compilation
CC=clang
CFLAGS+= -Wall -Wextra -Wpedantic \
         -Wformat=2 -Wno-unused-parameter -Wshadow \
         -Wwrite-strings -Wstrict-prototypes -Wold-style-definition \
         -Wredundant-decls -Wnested-externs -Wmissing-include-dirs
CFLAGS+= -std=c11

# Folders
SRC=src
OBJ=obj
BIN=bin
TEST=test
TEST_BIN=$(TEST)/bin

# Targets
BINS=$(BIN)/main
TEST_BINS=$(patsubst $(TEST)/%.c, $(TEST_BIN)/%, $(TESTS))

# Files
BIN_OBJS=$(patsubst $(BIN)/%, $(OBJ)/%.o, $(BINS))
SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(filter-out $(BIN_OBJS), $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS)))
TESTS=$(wildcard $(TEST)/*.c)

# Compilation rules
.SECONDARY: $(OBJS) $(BIN_OBJS)

all: $(BINS)

$(BIN)/%: $(OBJ)/%.o $(OBJS) | $(BIN)
	$(CC) $(CFLAGS) $^ -o $@

$(TEST_BIN)/%: $(TEST)/%.c $(OBJS) | $(TEST_BIN)
	$(CC) $(CFLAGS) $^ -o $@ -lcriterion

# For files with .h counterpart
$(OBJ)/%.o: $(SRC)/%.c $(SRC)/%.h | $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

# For files without .h counterpart
$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

# Create folders if they don't exist
$(OBJ):
	mkdir $@

$(BIN):
	mkdir $@

$(TEST_BIN):
	mkdir $@

# Pseudo-targets
.PHONY: test clean install-hooks run-hooks lint

test: $(TEST_BINS)
	for test_file in $^; do ./$$test_file; done

clean:
	rm -rf $(OBJ)
	rm -rf $(BIN)
	rm -rf $(TEST_BIN)

install-hooks:
	pre-commit install
	pre-commit install --hook-type commit-msg

run-hooks:
	pre-commit run --all-files

lint:
	clang-tidy $(SRCS) $(patsubst $(OBJ)/%.o, $(SRC)/%.h, $(OBJS)) $(TESTS)
