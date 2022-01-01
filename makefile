CFLAGS=-pedantic -Wall -Wextra -ggdb

parser: parser.o store.o main.o alex.o
	$(CC) $(CFLAGS) $^ -o $@ 

parser.o: parser.c 
	$(CC) $(CFLAGS) -c $^ -o $@

store.o: store.c
	$(CC) $(CFLAGS) -c $^ -o $@

main.o: main.c
	$(CC) $(CFLAGS) -c $^ -o $@

alex.o: alex.c 
	$(CC) $(CFLAGS) -c $^ -o $@


.PHONY: clean

clean:
	-rm *.o parser
