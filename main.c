#include <chrono>
#include <stdio.h>
#include "sudoku.h"

int main(int argc, char **argv) {

  SudokuBoard *board = sudoku_load(argv[1]);

  auto start = std::chrono::high_resolution_clock::now();
  if (!strcmp(argv[2], "0")) {
    printf("Breadth first\n");
    sudoku_breadth_first(board);
  }
  else if (!strcmp(argv[2], "1")) {
    printf("Depth first\n");
    sudoku_depth_first(board);
  }
  else if (!strcmp(argv[2], "2")) {
    printf("A Star\n");
    sudoku_a_star(board);
  }
    
  auto end = std::chrono::high_resolution_clock::now();
  printf("Solution in %d nanoseconds\n", std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());


  sudoku_display(board);
  sudoku_destroy(board);

  return 0;
}
