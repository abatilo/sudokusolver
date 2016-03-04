#include <stdio.h>
#include "queue.h"

int main() {
  Queue *queue = queue_create();
  queue_destroy(queue);

  printf("Hello world!\n");
  return 0;
}
