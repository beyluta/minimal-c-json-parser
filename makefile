.PHONY: release debug

CC = gcc
OUT = out
SRC = tests.c \
			src/json.c
DEPS = -Iinclude
CFLAGS = -xc \
				 -std=c23
ERRFLAGS = -Wall \
					-Wextra \
					-Werror

release:
	$(CC) $(CFLAGS) $(DEPS) $(SRC) $(ERRFLAGS) -o $(OUT)
debug:
	$(CC) $(CFLAGS) $(DEPS) $(SRC) -o $(OUT)
