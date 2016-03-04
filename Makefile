all: sudokusolver

sudokusolver: main.o
	gcc main.o -o sudokusolver

main.o: main.c queue.h
	gcc -c main.c

clean:
	rm -f *.o && rm -f sudokusolver*
