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
        *code = FULL_QUEUE;
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
        *code = EMPTY_QUEUE;
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
        *code = EMPTY_QUEUE;
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

void print_dump_queue_array(char *type, queue_a *s)
{
    printf("********************************\n");
    printf("QUEUE DUMP\n");
    printf("%s", type);
    if (!is_empty_a(s))
    {
        printf("%16s\n", "DATA");
        for (int i = 0; i < s->size; i++)
            printf("%16.2f\n", s->data[i]->add_time);
    }
    else
    {
        printf("%16s\n", "queue is empty");
    }
    printf("********************************\n");
}
