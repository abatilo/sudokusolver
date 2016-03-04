#ifndef STACK_H_
#define STACK_H_

// For fix width integer types
#include <stdint.h>
// For output
#include <stdio.h>
// For memory allocation
#include <stdlib.h>
// For memmove
#include <string.h>

const size_t STACK_START_SIZE = 8;
const size_t STACK_GROWTH_FACTOR = 2;
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
  for (i = 0; i < stack->tail; ++i) printf("%d ", stack->data[i]);
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

#endif
