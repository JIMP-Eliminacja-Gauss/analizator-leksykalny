CFLAGS=-pedantic -Wall -Wextra -ggdb

parser: parser.o store.o fun_stack.o alex.o
	$(CC) $(CFLAGS) $^ -o $@ 

parser.o: parser.c 
	$(CC) $(CFLAGS) -c $^ -o $@

store.o: store.c
	$(CC) $(CFLAGS) -c $^ -o $@

fun_stack.o: fun_stack.c
	$(CC) $(CFLAGS) -c $^ -o $@

alex.o: alex.c 
	$(CC) $(CFLAGS) -c $^ -o $@


.PHONY: clean

clean:
	-rm *.o parser
