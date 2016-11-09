#ifndef QUEUE_LIST_H
#define QUEUE_LIST_H


#include "types.h"
#include "queue_errors.h"

struct _object_t
{
    application_t *data;
    struct _object_t *next;
};
typedef struct _object_t object_t;

typedef struct
{
    object_t *head;
    object_t *tail;
    int size;
} queue_l;

object_t *create_object(application_t *a);
void destroy_object(object_t *o);

queue_l *create_queue_l();
void destroy_queue_l(queue_l *q);

void enqueue_l(queue_l *q, application_t *a, int *code);
application_t *dequeue_l(queue_l *q, int *code);
application_t *peek_l(queue_l *q, int *code);
int size_l(queue_l *q);
int is_empty_l(queue_l *q);

#endif // QUEUE_LIST_H
