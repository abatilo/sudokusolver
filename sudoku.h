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

const size_t LIST_START_SIZE = 16;
const size_t LIST_GROWTH_FACTOR = 2;

const size_t STACK_START_SIZE = 16;
const size_t STACK_GROWTH_FACTOR = 2;

const size_t QUEUE_START_SIZE = 16;
const size_t QUEUE_GROWTH_FACTOR = 2;

typedef struct {
  // Used for A* search
  int32_t priority;
  // Board data
  char board[9][9];
} SudokuBoard;

// Signature for sudoku_display
void sudoku_display(SudokuBoard *board);
int32_t sudoku_compare_to(SudokuBoard *original, SudokuBoard *other);
void sudoku_find_priority(SudokuBoard *board);

typedef struct {

  // Index for end of head
  int32_t tail;
  // The number of elements in the list
  int32_t size;
  // The number of elements the list can hold
  int32_t capacity;
  // The list data
  int32_t *data;
} List;

typedef struct {

  // Index for end of head
  int32_t tail;
  // The number of elements in the list
  int32_t size;
  // The number of elements the list can hold
  int32_t capacity;
  // The list data
  SudokuBoard **data;
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

typedef struct {
  int32_t size;
  Stack *data;
} MinHeap;

List *list_create() {
  List *list = (List *)malloc(sizeof(List));
  list->tail = 0;
  list->size = 0;
  list->capacity = LIST_START_SIZE;
  list->data = (int32_t *)calloc(list->capacity, sizeof(int32_t));
  return list;
}

void list_destroy(List *list) {
  free(list->data);
  free(list);
}

void list_display(List *list) {
  size_t i;
  for (i = 0; i < list->tail; ++i) printf("%c ", list->data[i]);
  printf("\n");
}

void list_insert(List *list, int32_t data) {
  // If the end of the list is as large as its max capacity
  // then we need to resize the list to hold more data
  if (list->tail == list->capacity) {
    int32_t new_capacity = LIST_GROWTH_FACTOR * list->capacity;
    // Store in temp to validate a successful realloc
    int32_t *temp_data = (int32_t *)realloc(list->data, new_capacity * sizeof(int32_t));

    // Successful realloc
    if (temp_data != NULL) {
      list->data = temp_data;
      list->capacity = new_capacity;
    }
    else {
      // TODO(aaron): Do something better here instead of just failing out
      return;
    }
  }

  list->data[list->tail] = data;
  ++list->tail;
  ++list->size;
}

// Note that we don't have a list_remove. This is on purpose. I never need it
// in this program

Stack *stack_create() {
  Stack *stack = (Stack *)malloc(sizeof(Stack));
  stack->tail = 0;
  stack->size = 0;
  stack->capacity = STACK_START_SIZE;
  stack->data = (SudokuBoard **)malloc(stack->capacity * sizeof(SudokuBoard *));
  return stack;
}

void stack_destroy(Stack *stack) {
  size_t i;
  for (i = 0; i < stack->size; ++i) free(stack->data[i]);
  free(stack->data);
  free(stack);
}

void stack_display(Stack *stack) {
  size_t i;
  for (i = 0; i < stack->tail; ++i) sudoku_display(stack->data[i]);
  printf("\n");
}

void stack_push(Stack *stack, SudokuBoard *data) {
  // If the end of the stack is as large as its max capacity
  // then we need to resize the stack to hold more data
  if (stack->tail == stack->capacity) {
    int32_t new_capacity = STACK_GROWTH_FACTOR * stack->capacity;
    // Store in temp to validate a successful realloc
    SudokuBoard **temp_data = (SudokuBoard **)realloc(stack->data, new_capacity * sizeof(SudokuBoard *));

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

SudokuBoard *stack_pop(Stack *stack) {
  SudokuBoard *data = stack->data[stack->tail - 1];
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

MinHeap *minheap_create() {
  MinHeap *heap = (MinHeap *)malloc(sizeof(MinHeap));
  heap->size = 0;
  heap->data = stack_create();
  return heap;
}

void minheap_destroy(MinHeap *heap) {
  stack_destroy(heap->data);
  free(heap);
}

void minheap_add(MinHeap *heap, SudokuBoard *board) {
  stack_push(heap->data, board);
  heap->size = heap->data->size;
  int32_t c = heap->data->size - 1;

  while (c > 0 && sudoku_compare_to(heap->data->data[c], heap->data->data[c / 2]) == -1) {
    SudokuBoard *temp = heap->data->data[c];
    heap->data->data[c] = heap->data->data[c / 2];
    heap->data->data[c / 2] = temp;
    c /= 2;
  }
}

SudokuBoard *minheap_remove_min(MinHeap* heap) {
  SudokuBoard *board = heap->data->data[0];
  heap->data->data[0] = stack_pop(heap->data);
  heap->size = heap->data->size;

  int c = 0;
  while (c < heap->data->size) {
    int min = c;
    if (2 * c + 1 < heap->data->size && sudoku_compare_to(heap->data->data[2 * c + 1], heap->data->data[min]) == -1)
      min = 2 * c + 1;
    if (2 * c + 2 < heap->data->size && sudoku_compare_to(heap->data->data[2 * c + 2], heap->data->data[min]) == -1)
      min = 2 * c + 2;

    if (min == c) break;
    else {
      SudokuBoard *temp = heap->data->data[c];
      heap->data->data[c] = heap->data->data[c / 2];
      heap->data->data[c / 2] = temp;
      c = min;
    }
  }

  return board;
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
  sudoku_find_priority(board);
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
  sudoku_find_priority(board);
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
  sudoku_find_priority(dest);
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

List *sudoku_possible_cell_values(SudokuBoard *board, int32_t row, int32_t col) {
  List *list = list_create();

  // Make sure there's not something there already
  if (board->board[row][col] != BOARD_EMPTY) {
    return list;
  }

  size_t i;
  for (i = 0; i < BOARD_SIZE; ++i) {
    board->board[row][col] = (char)('1' + i);
    if (sudoku_is_row_valid(board, row) &&
        sudoku_is_column_valid(board, col) &&
        sudoku_is_block_valid(board, row, col)) {
      list_insert(list, board->board[row][col]);
    }
  }

  board->board[row][col] = BOARD_EMPTY;
  return list;
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

int32_t sudoku_compare_to(SudokuBoard *original, SudokuBoard *other) {
  if (original->priority < other->priority) return -1;
  else if (original->priority > other->priority) return 1;
  else return 0;
}

void sudoku_find_priority(SudokuBoard *board) {
  int32_t priority = 0;
  size_t i, j;
  for (i = 0; i < BOARD_SIZE; ++i) {
    for (j = i; j < BOARD_SIZE; ++j) {
      if (board->board[i][j] == BOARD_EMPTY) {
        int32_t size = sudoku_possible_cell_values(board, i, j)->size;
        priority += size * size;
      }
    }
  }
  board->priority = priority;
}

void sudoku_breadth_first(SudokuBoard *start) {
  int32_t row;
  int32_t col;
  SudokuBoard *sudoku = NULL;
  const int32_t upper_limit = 5000000;
  size_t number_nodes_visited = 0;
  size_t max_queue_size = 0;

  Stack *to_visit = stack_create();
  SudokuBoard *copy = sudoku_create();
  sudoku_copy(copy, start);
  stack_push(to_visit, copy);

  while (to_visit->size > 0 && to_visit->size < upper_limit) {

    ++number_nodes_visited;
    if (to_visit->size > max_queue_size) {
      max_queue_size = to_visit->size;
    }

    sudoku = stack_pop(to_visit);
    sudoku_find_empty_cell(sudoku, &row, &col);

    size_t i;
    List *possible_cell_values = sudoku_possible_cell_values(sudoku, row, col);
    for (i = 0; i < possible_cell_values->size; ++i) {

      // Need to deallocate these
      // These get added to to_visit, so as long as we clear to_visit
      // these point to the same things
      SudokuBoard *new_board = sudoku_create();
      sudoku_copy(new_board, sudoku);
      sudoku_set_cell_value(new_board, row, col, possible_cell_values->data[i]);

      stack_push(to_visit, new_board);

      int32_t j;
      int32_t k;
      sudoku_find_empty_cell(new_board, &j, &k);

      if (j == -1 && k == -1) {
        printf("Size of Queue: %d\n", max_queue_size);
        printf("Nodes visited: %d\n", number_nodes_visited);
        sudoku_copy(start, new_board);

        sudoku_destroy(new_board);
        sudoku_destroy(sudoku);
        stack_destroy(to_visit);
        return;
      }
    }
    list_destroy(possible_cell_values);
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

    size_t i;
    List *possible_cell_values = sudoku_possible_cell_values(sudoku, row, col);
    for (i = 0; i < possible_cell_values->size; ++i) {

      // Need to deallocate these
      // These get added to to_visit, so as long as we clear to_visit
      // these point to the same things
      SudokuBoard *new_board = sudoku_create();
      sudoku_copy(new_board, sudoku);
      sudoku_set_cell_value(new_board, row, col, possible_cell_values->data[i]);

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
    list_destroy(possible_cell_values);
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

void sudoku_a_star(SudokuBoard *start) {
  int32_t row;
  int32_t col;
  SudokuBoard *sudoku = NULL;
  const int32_t upper_limit = 5000000;
  size_t number_nodes_visited = 0;
  size_t max_queue_size = 0;

  MinHeap *to_visit = minheap_create();
  SudokuBoard *copy = sudoku_create();
  sudoku_copy(copy, start);
  minheap_add(to_visit, copy);

  while (to_visit->size > 0 && to_visit->size < upper_limit) {

    ++number_nodes_visited;
    if (to_visit->size > max_queue_size) {
      max_queue_size = to_visit->size;
    }

    sudoku = minheap_remove_min(to_visit);

    int32_t prunable = 0;
    do {
      prunable = 0;
      size_t i, j;
      for (i = 0; i < BOARD_SIZE; ++i) {
        for (j = i; j < BOARD_SIZE; ++j) {
          List *possible = sudoku_possible_cell_values(sudoku, i, j);
          if (possible->size == 1) {
            ++prunable;
            sudoku_set_cell_value(sudoku, i, j, possible->data[0]);
          }
          list_destroy(possible);

          possible = sudoku_possible_cell_values(sudoku, j, i);
          if (possible->size == 1) {
            ++prunable;
            sudoku_set_cell_value(sudoku, j, i, possible->data[0]);
          }
          list_destroy(possible);
        }
      }
    } while (prunable > 0);

    sudoku_find_empty_cell(sudoku, &row, &col);
    if (row == -1 && col == -1) {
      printf("Size of Queue: %d\n", max_queue_size);
      printf("Nodes visited: %d\n", number_nodes_visited);
      sudoku_copy(start, sudoku);

      sudoku_destroy(sudoku);
      minheap_destroy(to_visit);
      return;
    }

    size_t i;
    List *possible_cell_values = sudoku_possible_cell_values(sudoku, row, col);
    for (i = 0; i < possible_cell_values->size; ++i) {
      // Need to deallocate these
      // These get added to to_visit, so as long as we clear to_visit
      // these point to the same things
      SudokuBoard *new_board = sudoku_create();
      sudoku_copy(new_board, sudoku);
      sudoku_set_cell_value(new_board, row, col, possible_cell_values->data[i]);

      minheap_add(to_visit, new_board);

      int32_t j;
      int32_t k;
      sudoku_find_empty_cell(new_board, &j, &k);

      if (j == -1 && k == -1) {
        printf("Size of Queue: %d\n", max_queue_size);
        printf("Nodes visited: %d\n", number_nodes_visited);
        sudoku_copy(start, new_board);

        sudoku_destroy(new_board);
        sudoku_destroy(sudoku);
        minheap_destroy(to_visit);
        return;
      }
    }
    list_destroy(possible_cell_values);
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
