# Compilation Flags
CC := clang
CFLAGS += -Wall -Wextra -Wpedantic \
					-Wformat=2 -Wno-unused-parameter -Wshadow \
					-Wwrite-strings -Wstrict-prototypes -Wold-style-definition \
					-Wredundant-decls -Wnested-externs -Wmissing-include-dirs
CFLAGS += -std=c11

# Targets
TARGET := rastreador

# Files
SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c, %.o, $(SRCS))

# Compilation rules
.SECONDARY: $(OBJS)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Pseudo-targets
.PHONY: clean install-hooks run-hooks

clean:
	rm -f $(OBJS)
	rm -f $(TARGET)

install-hooks:
	pre-commit install
	pre-commit install --hook-type commit-msg

run-hooks:
	pre-commit run --all-files
