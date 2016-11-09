#ifndef QUEUE_ARRAY_H
#define QUEUE_ARRAY_H

#include "types.h"
#include "queue_errors.h"

#define MAX_SIZE 1000

typedef struct
{
    application_t *data[MAX_SIZE];
    int head;
    int tail;
    int size;
} queue_a;

queue_a *create_queue_a();
void destroy_queue_a(queue_a *q);

void enqueue_a(queue_a *q, application_t *a, int *code);
application_t *dequeue_a(queue_a *q, int *code);
application_t *peek_a(queue_a *q, int *code);
int size_a(queue_a *q);
int is_empty_a(queue_a *q);

#endif // QUEUE_ARRAY_H
