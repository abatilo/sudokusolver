#include <stdio.h>
#include "stack.h"
#include "stack.h"

int main() {
  Stack *stack = stack_create();

  size_t i;
  for (i = 0; i < 8; ++i) {
    stack_push(stack, (i + 1));
    stack_display(stack);
  }

  for (i = 0; i < 4; ++i) {
    printf("%d\n", stack_pop(stack));
  }

  for (i = 0; i < 12; ++i) {
    stack_push(stack, (i + 1));
    stack_display(stack);
  }

  stack_destroy(stack);

  return 0;
}
