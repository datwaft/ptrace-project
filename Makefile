# Compilation
CC := clang
CFLAGS += -Wall -Wextra -Wpedantic \
					-Wformat=2 -Wno-unused-parameter -Wshadow \
					-Wwrite-strings -Wstrict-prototypes -Wold-style-definition \
					-Wredundant-decls -Wnested-externs -Wmissing-include-dirs
CFLAGS += -std=c11

# Targets
BINS := rastreador

# Files
BIN_OBJS := $(patsubst %, %.o, $(BINS))
SRCS := $(wildcard *.c)
OBJS := $(filter-out $(BIN_OBJS), $(patsubst %.c, %.o, $(SRCS)))

# Compilation rules
.SECONDARY: $(OBJS) $(BIN_OBJS)

all: $(BINS)

$(BIN)/%: $(OBJ)/%.o $(OBJS) | $(BIN)
	$(CC) $(CFLAGS) $^ -o $@

# For files without .h counterpart
$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

# Pseudo-targets
.PHONY: clean install-hooks run-hooks 

clean:
	rm $(BIN_OBJS)
	rm $(BINS)

install-hooks:
	pre-commit install
	pre-commit install --hook-type commit-msg

run-hooks:
	pre-commit run --all-files


