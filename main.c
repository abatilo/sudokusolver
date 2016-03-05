#include <chrono>
#include <stdio.h>
#include "sudoku.h"

int main(int argc, char **argv) {

  SudokuBoard *board = sudoku_load(argv[1]);

  auto start = std::chrono::high_resolution_clock::now();
  sudoku_depth_first(board);
  auto end = std::chrono::high_resolution_clock::now();
  printf("Solution in %d milliseconds\n", std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());


  sudoku_display(board);
  sudoku_destroy(board);

  return 0;
}
