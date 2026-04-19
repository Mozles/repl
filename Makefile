CC := gcc
CFLAGS := -Wall -Wextra -std=c99

.PHONY: run
run: main
	./main

main: main.c
	$(CC) -o $@ $^ $(CFLAGS)
