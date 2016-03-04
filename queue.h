#ifndef QUEUE_H_
#define QUEUE_H_

// For fix width integer types
#include <stdint.h>
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

#endif
