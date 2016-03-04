#include <stdio.h>
#include "queue.h"

int main() {
  Queue *queue = queue_create();

  size_t i;
  for (i = 0; i < 10; ++i) {
    queue_enqueue(queue, (i + 1));
    queue_display(queue);
  }

  for (i = 0; i < 5; ++i) queue_dequeue(queue);
  queue_display(queue);
  for (i = 0; i < 5; ++i) queue_enqueue(queue, (i + 1));
  queue_display(queue);

  queue_destroy(queue);

  return 0;
}
