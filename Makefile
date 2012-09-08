CC=gcc
CFLAGS=-O0 -g

tester: tester.o rc4.o
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f *.o tester

check:
	cat input.txt | ./tester 44-55-66-77-88 | ./tester 44-55-66-77-88
