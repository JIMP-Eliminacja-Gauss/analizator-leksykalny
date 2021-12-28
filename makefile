CFLAGS=-pedantic -Wall -Wextra -ggdb

parser: store.o test.o
	$(CC) $(CFLAGS) $^ -o $@ 

parser.o: parser.c 
	$(CC) $(CFLAGS) -c $^ -o $@

store.o: store.c
	$(CC) $(CFLAGS) -c $^ -o $@

fun_stack.o: fun_stack.c
	$(CC) $(CFLAGS) -c $^ -o $@

alex.o: alex.c 
	$(CC) $(CFLAGS) -c $^ -o $@

test.o: test.c
	$(CC) $(CFLAGS) -c $^ -o $@



.PHONY: clean

clean:
	-rm *.o parser
