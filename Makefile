all: sudokusolver

sudokusolver: main.o
	g++ main.o -o sudokusolver

main.o: sudoku.h main.c
	g++ -O3 -std=c++11 -c main.c

clean:
	rm -f *.o && rm -f sudokusolver*
