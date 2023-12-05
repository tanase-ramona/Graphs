CC = gcc
CFLAGS = -g -Wall -lm -std=c99

build: tema3

tema3: tema3.c
	$(CC) -o tema3 tema3.c $(CFLAGS)

run: tema3
	./tema3 "$(ARG)"

.PHONY : clean
clean :
	rm -f tema3
	rm -f *.out
	rm my_diff
