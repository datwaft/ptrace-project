# Compilation
CC := clang
CFLAGS += -Wall -Wextra -Wpedantic \
					-Wformat=2 -Wno-unused-parameter -Wshadow \
					-Wwrite-strings -Wstrict-prototypes -Wold-style-definition \
					-Wredundant-decls -Wnested-externs -Wmissing-include-dirs
CFLAGS += -std=c11

# Targets
TARGET := rastreador

# Files
OBJS := $(patsubst %, %.o, $(TARGET))
SRCS := $(wildcard *.c)

# Compilation rules
.SECONDARY: $(OBJS)

all: $(TARGET)

$(TARGET)/%: $(OBJS) | $(TARGET)
	$(CC) $(CFLAGS) $^ -o $@

$(TARGET)/%.o: $(SRC)/%.c | $(OBJS)
	$(CC) $(CFLAGS) -c $< -o $@

# Pseudo-TARGETs
.PHONY: clean install-hooks run-hooks 

clean:
	rm -f $(OBJS)
	rm -f $(TARGET)

install-hooks:
	pre-commit install
	pre-commit install --hook-type commit-msg

run-hooks:
	pre-commit run --all-files