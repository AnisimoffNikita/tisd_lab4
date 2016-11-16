#include "queue_list.h"

#include <malloc.h>
#include "garbage.h"

object_t *create_object(application_t *a)
{
    object_t *ptr = malloc(sizeof(object_t));
    ptr->data = a;
    ptr->next = NULL;
    return ptr;
}

void destroy_object(object_t *o)
{
    free(o);
}

queue_l *create_queue_l()
{
    queue_l *ptr = malloc(sizeof(queue_l));
    ptr->head = ptr->tail = NULL;
    ptr->size = 0;
    return ptr;
}

void destroy_queue_l(queue_l *q)
{
    free(q);
}

void enqueue_l(queue_l *q, application_t *a, int *code)
{
    *code = OK;
    q->size++;
    object_t *o = create_object(a);
    delete_freed(o);
    if (q->tail)
    {
        q->tail->next = o;
        q->tail = o;
    }
    else
        q->tail = q->head = o;
}

application_t *dequeue_l(queue_l *q, int *code)
{
    if (q->size == 0)
    {
        *code = EMPTY_QUEUE;
        return NULL;
    }
    q->size--;
    application_t *a = q->head->data;
    dump_freed(q->head);
    destroy_object(q->head);
    q->head = q->head->next;
    if (!q->head) q->tail = q->head;
    return a;
}

application_t *peek_l(queue_l *q, int *code)
{
    if (q->size == 0)
    {
        *code = EMPTY_QUEUE;
        return NULL;
    }
    return q->head->data;
}

int size_l(queue_l *q)
{
    return q->size;
}

int is_empty_l(queue_l *q)
{
    return q->size == 0;
}

void print_dump_queue_list(char *type, queue_l *s)
{
    printf("********************************\n");
    printf("QUEUE DUMP\n");
    printf("%s", type);
    if (!is_empty_l(s))
    {
        printf("%16s%16s\n", "DATA", "POINTER");
        object_t *top = s->head;
        while (top)
        {
            printf("%16.3f%16p\n", top->data->add_time, (void*)top);
            top = top->next;
        }
    }
    else
    {
        printf("%32s\n", "queue is empty");
    }
    print_freed();
    printf("********************************\n");
}


