#ifndef SUDOKU_H_ 
#define SUDOKU_H_ 

// For booleans
#include <stdbool.h>
// For fix width integer types
#include <stdint.h>
// For output
#include <stdio.h>
// For memory allocation
#include <stdlib.h>
// For memmove
#include <string.h>

const size_t BOARD_SIZE = 9;
// In C we can't initialize start with BOARD_SIZE * BOARD_SIZE
const char BOARD_EMPTY = '_';

const size_t STACK_START_SIZE = 16;
const size_t STACK_GROWTH_FACTOR = 2;

const size_t QUEUE_START_SIZE = 16;
const size_t QUEUE_GROWTH_FACTOR = 2;

typedef struct {
  char board[9][9];
} SudokuBoard;

typedef struct {

  // Index for end of head
  int32_t tail;
  // The number of elements in the stack
  int32_t size;
  // The number of elements the stack can hold
  int32_t capacity;
  // The stack data
  int32_t *data;
} Stack;

typedef struct {
  // Index for start of queue
  int32_t head;
  // Index for end of head
  int32_t tail;
  // The number of elements in the queue
  int32_t size;
  // The number of elements the queue can hold
  int32_t capacity;
  // The queue data
  SudokuBoard **data;
} Queue;


Stack *stack_create() {
  Stack *stack = (Stack *)malloc(sizeof(Stack));
  stack->tail = 0;
  stack->size = 0;
  stack->capacity = STACK_START_SIZE;
  stack->data = (int32_t *)calloc(stack->capacity, sizeof(int32_t));
  return stack;
}

void stack_destroy(Stack *stack) {
  free(stack->data);
  free(stack);
}

void stack_display(Stack *stack) {
  size_t i;
  for (i = 0; i < stack->tail; ++i) printf("%c ", stack->data[i]);
  printf("\n");
}

void stack_push(Stack *stack, int32_t data) {
  // If the end of the stack is as large as its max capacity
  // then we need to resize the stack to hold more data
  if (stack->tail == stack->capacity) {
    int32_t new_capacity = STACK_GROWTH_FACTOR * stack->capacity;
    // Store in temp to validate a successful realloc
    int32_t *temp_data = (int32_t *)realloc(stack->data, new_capacity * sizeof(int32_t));

    // Successful realloc
    if (temp_data != NULL) {
      stack->data = temp_data;
      stack->capacity = new_capacity;
    }
    else {
      // TODO(aaron): Do something better here instead of just failing out
      return;
    }
  }

  stack->data[stack->tail] = data;
  ++stack->tail;
  ++stack->size;
}

int32_t stack_pop(Stack *stack) {
  int32_t data = stack->data[stack->tail - 1];
  --stack->tail;
  --stack->size;
  return data;
}

Queue *queue_create() {
  Queue *queue = (Queue *)malloc(sizeof(Queue));
  queue->head = 0;
  queue->tail = 0;
  queue->size = 0;
  queue->capacity = QUEUE_START_SIZE;
  queue->data = (SudokuBoard **)malloc(queue->capacity * sizeof(SudokuBoard *));
  return queue;
}

void queue_destroy(Queue *queue) {
  size_t i;
  for (i = queue->head; i < queue->tail; ++i) free(queue->data[i]);
  free(queue->data);
  free(queue);
}

// Signature for sudoku_display
void sudoku_display(SudokuBoard *board);
void queue_display(Queue *queue) {
  size_t i;
  for (i = queue->head; i < queue->tail; ++i) sudoku_display(queue->data[i]);
  printf("\n");
}

void queue_enqueue(Queue *queue, SudokuBoard *data) {
  // If the end of the queue is as large as its max capacity
  // then we need to resize the queue to hold more data
  if (queue->tail == queue->capacity) {
    int32_t new_capacity = QUEUE_GROWTH_FACTOR * queue->capacity;
    // Store in temp to validate a successful realloc
    SudokuBoard **temp_data = (SudokuBoard **)realloc(queue->data, new_capacity * sizeof(SudokuBoard *));

    // Successful realloc
    if (temp_data != NULL) {
      queue->data = temp_data;
      queue->capacity = new_capacity;

      // On resize remove invalid data by moving memory over old data
      memmove(queue->data, queue->data + queue->head, queue->size * sizeof(SudokuBoard *));
      queue->tail -= queue->head;
      queue->head = 0;
    }
    else {
      // TODO(aaron): Do something better here instead of just failing out
      return;
    }
  }

  queue->data[queue->tail] = data;
  ++queue->tail;
  ++queue->size;
}

SudokuBoard *queue_dequeue(Queue *queue) {
  SudokuBoard *data = queue->data[queue->head];
  ++queue->head;
  --queue->size;
  return data;
}

SudokuBoard *sudoku_create() {
  SudokuBoard *board = (SudokuBoard *)malloc(sizeof(SudokuBoard));
  //board->board[9][9] = { '_' };

  // Is start necessary? Can we assume we won't need to initialize a blank
  // board?
  size_t i, j;
  for (i = 0; i < BOARD_SIZE; ++i) {
    for (j = 0; j < BOARD_SIZE; ++j) {
      board->board[i][j] = BOARD_EMPTY;
    }
  }
  return board;
}

void sudoku_destroy(SudokuBoard *board) {
  free(board);
}

SudokuBoard *sudoku_load(char *path) {
  SudokuBoard *board = sudoku_create();
  FILE *board_file = fopen(path, "r");

  size_t i, j;
  for (i = 0; i < BOARD_SIZE; ++i) {
    for (j = 0; j < BOARD_SIZE; ++j) {
      char c = fgetc(board_file);
      board->board[i][j] = c;
      // Skip the space
      fgetc(board_file);
    }
  }

  return board;
}

void sudoku_display(SudokuBoard *board) {
  printf("---------------------\n");
  size_t i, j;
  for (i = 0; i < BOARD_SIZE; ++i) {
    printf("| ");
    for (j = 0; j < BOARD_SIZE; ++j) {
      printf("%c ", board->board[i][j]);
    }
    printf("|\n");
  }
  printf("---------------------\n");
}

// A "copy constructor"
void sudoku_copy(SudokuBoard *dest, SudokuBoard *src) {
  size_t i, j;
  for (i = 0; i < BOARD_SIZE; ++i) {
    for (j = 0; j < BOARD_SIZE; ++j) {
      dest->board[i][j] = src->board[i][j];
    }
  }
}

void sudoku_set_cell_value(SudokuBoard *board, uint32_t row, uint32_t col, char value) {
  board->board[row][col] = value;
}

bool sudoku_is_block_valid(SudokuBoard *board, int32_t row, int32_t col) {
  int32_t block_row = 3 * (row / 3);
  int32_t block_col = 3 * (col / 3);

  int32_t i_row;
  int32_t i_col;
  int32_t j_row;
  int32_t j_col;

  size_t i;
  for (i = 0; i < BOARD_SIZE - 1; ++i) {
    i_row = block_row + i / 3;
    i_col = block_col + i % 3;

    size_t j;
    for (j = i + 1; j < BOARD_SIZE; ++j) {
      j_row = block_row + j / 3;
      j_col = block_col + j % 3;

      if (board->board[i_row][i_col] != BOARD_EMPTY &&
          board->board[i_row][i_col] == board->board[j_row][j_col]) {
        return false;
      }
    }
  }

  return true;
}

bool sudoku_is_column_valid(SudokuBoard *board, uint32_t col) {
  size_t i, j;
  for (i = 0; i < BOARD_SIZE - 1; ++i)
    for (j = i + 1; j < BOARD_SIZE; ++j)
      if (board->board[i][col] != BOARD_EMPTY &&
          board->board[i][col] == board->board[j][col]) return false;
  return true;
}

bool sudoku_is_row_valid(SudokuBoard *board, uint32_t row) {
  size_t i, j;
  for (i = 0; i < BOARD_SIZE - 1; ++i)
    for (j = i + 1; j < BOARD_SIZE; ++j)
      if (board->board[row][i] != BOARD_EMPTY &&
          board->board[row][i] == board->board[row][j]) return false;
  return true;
}

bool sudoku_is_board_valid(SudokuBoard *board) {
  size_t i, j;
  for (i = 0; i < BOARD_SIZE; ++i) {
    for (j = 0; j < BOARD_SIZE; ++j) {
      if (!sudoku_is_row_valid(board, i) || !sudoku_is_column_valid(board, j) || !sudoku_is_block_valid(board, i, j)) {
        return false;
      }
    }
  }
  return true;
}

Stack *sudoku_possible_cell_values(SudokuBoard *board, int32_t row, int32_t col) {
  Stack *stack = stack_create();

  // Make sure there's not something there already
  if (board->board[row][col] != BOARD_EMPTY) {
    return stack;
  }

  size_t i;
  for (i = 0; i < BOARD_SIZE; ++i) {
    board->board[row][col] = (char)('1' + i);
    if (sudoku_is_row_valid(board, row) &&
        sudoku_is_column_valid(board, col) &&
        sudoku_is_block_valid(board, row, col)) {
      stack_push(stack, board->board[row][col]);
    }
  }

  board->board[row][col] = BOARD_EMPTY;
  return stack;
}

void sudoku_find_empty_cell(SudokuBoard *board, int32_t *row, int32_t *col) {
  size_t i, j;
  for (i = 0; i < BOARD_SIZE; ++i) {
    for (j = 0; j < BOARD_SIZE; ++j) {
      if (board->board[i][j] == BOARD_EMPTY) {
        *row = i;
        *col = j;
        return;
      }
    }
  }
  *row = -1;
  *col = -1;
}

void sudoku_depth_first(SudokuBoard *start) {
  int32_t row;
  int32_t col;
  SudokuBoard *sudoku = NULL;
  const int32_t upper_limit = 5000000;
  size_t number_nodes_visited = 0;
  size_t max_queue_size = 0;

  Queue *to_visit = queue_create();
  SudokuBoard *copy = sudoku_create();
  sudoku_copy(copy, start);
  queue_enqueue(to_visit, copy);

  while (to_visit->size > 0 && to_visit->size < upper_limit) {

    ++number_nodes_visited;
    if (to_visit->size > max_queue_size) {
      max_queue_size = to_visit->size;
    }

    sudoku = queue_dequeue(to_visit);
    sudoku_find_empty_cell(sudoku, &row, &col);

    //printf("%d\n", number_nodes_visited);
    //sudoku_display(active);

    size_t i;
    Stack *possible_cell_values = sudoku_possible_cell_values(sudoku, row, col);
    // printf("(%d, %d) has these following values: ", row, col);
    // stack_display(possible_cell_values);
    for (i = 0; i < possible_cell_values->size; ++i) {

      // Need to deallocate these
      // These get added to to_visit, so as long as we clear to_visit
      // these point to the same things
      SudokuBoard *new_board = sudoku_create();
      sudoku_copy(new_board, sudoku);
      sudoku_set_cell_value(new_board, row, col, possible_cell_values->data[i]);
      // printf("Set (%d, %d) to %c\n", row, col, possible_cell_values->data[i]);

      queue_enqueue(to_visit, new_board);

      int32_t j;
      int32_t k;
      sudoku_find_empty_cell(new_board, &j, &k);

      if (j == -1 && k == -1) {
        printf("Size of Queue: %d\n", max_queue_size);
        printf("Nodes visited: %d\n", number_nodes_visited);
        sudoku_copy(start, new_board);

        sudoku_destroy(new_board);
        sudoku_destroy(sudoku);
        queue_destroy(to_visit);
        return;
      }
    }
    stack_destroy(possible_cell_values);
    sudoku_destroy(sudoku);
  }

  if (to_visit->size == 0) {
    printf("No solution\n");
  }
  else if (to_visit->size >= upper_limit) {
    printf("Exceeded upper limit\n");
  }

  printf("Size of Queue: %d\n", max_queue_size);
  printf("Nodes visited: %d\n", number_nodes_visited);
}

#endif
