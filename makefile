.PHONY: build

CC = gcc
OUT = out
SRC = tests.c \
			src/json.c
DEPS = -Iinclude
CFLAGS = -xc \
				 -std=c23 \
				 -Wextra \
				 -Wall \
				 -Werror

run: build
	./$(OUT)
build:
	$(CC) $(CFLAGS) $(DEPS) $(SRC) -o $(OUT)
