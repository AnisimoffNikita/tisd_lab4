#include "queue_array.h"

#include <malloc.h>

queue_a *create_queue_a()
{
    queue_a *ptr = malloc(sizeof(queue_a));
    ptr->size = 0;
    ptr->head = ptr->tail = 0;
    return ptr;
}

void destroy_queue_a(queue_a *q)
{
    free(q);
}

void enqueue_a(queue_a *q, application_t *a, int *code)
{
    if (q->size == MAX_SIZE)
    {
        *code = FULL;
        return;
    }
    q->data[q->tail] = a;
    q->tail = (q->tail + 1) % MAX_SIZE;
    q->size++;
}

application_t *dequeue_a(queue_a *q, int *code)
{
    if (q->size == 0)
    {
        *code = EMPTY;
        return NULL;
    }
    application_t *a = q->data[q->head];
    q->head = (q->head + 1) % MAX_SIZE;
    q->size--;
    return a;
}

application_t *peek_a(queue_a *q, int *code)
{
    if (q->head == q->tail)
    {
        *code = EMPTY;
        return NULL;
    }
    application_t *a = q->data[q->head];
    return a;
}

int size_a(queue_a *q)
{
    return q->size;
}

int is_empty_a(queue_a *q)
{
    return q->size == 0;
}
