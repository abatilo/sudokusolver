#ifndef QUEUE_H_
#define QUEUE_H_

// For fix width integer types
#include <stdint.h>
// For output
#include <stdio.h>
// For memory allocation
#include <stdlib.h>
// For memmove
#include <string.h>

const size_t QUEUE_START_SIZE = 8;
const size_t QUEUE_GROWTH_FACTOR = 2;
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
  int32_t *data;
} Queue;

Queue *queue_create() {
  Queue *queue = (Queue *)malloc(sizeof(Queue));
  queue->head = 0;
  queue->tail = 0;
  queue->size = 0;
  queue->capacity = QUEUE_START_SIZE;
  queue->data = (int32_t *)calloc(queue->capacity, sizeof(int32_t));
  return queue;
}

void queue_destroy(Queue *queue) {
  free(queue->data);
  free(queue);
}

void queue_display(Queue *queue) {
  size_t i;
  for (i = queue->head; i < queue->tail; ++i) printf("%d ", queue->data[i]);
  printf("\n");
}

void queue_enqueue(Queue *queue, int32_t data) {
  // If the end of the queue is as large as its max capacity
  // then we need to resize the queue to hold more data
  if (queue->tail == queue->capacity) {
    int32_t new_capacity = QUEUE_GROWTH_FACTOR * queue->capacity;
    // Store in temp to validate a successful realloc
    int32_t *temp_data = (int32_t *)realloc(queue->data, new_capacity * sizeof(int32_t));

    // Successful realloc
    if (temp_data != NULL) {
      queue->data = temp_data;
      queue->capacity = new_capacity;

      // On resize remove invalid data by moving memory over old data
      memmove(queue->data, queue->data + queue->head, queue->size * sizeof(int32_t));
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

void queue_dequeue(Queue *queue) {
  ++queue->head;
  --queue->size;
}

#endif
