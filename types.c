#include "types.h"

#include <stdlib.h>
#include <malloc.h>

double rand_0_1()
{
    return (double)rand()/(double)RAND_MAX;
}

application_t *create_application(application_type_t type, float t1, float t2, float t3, float t4, float start_t)
{
    application_t *ptr = malloc(sizeof(application_t));
    ptr->type = type;
    ptr->add_time = (t2 - t1)*rand_0_1() + t1;
    ptr->process_time = (t4 - t3)*rand_0_1() + t3;
    ptr->start_time = start_t + ptr->add_time;
    return ptr;
}

void destroy_application(application_t *a)
{
    free(a);
}

event_t *create_event(application_t *a, float time, float type)
{
    event_t *ptr = malloc(sizeof(application_t));
    ptr->a = a;
    ptr->time = time;
    ptr->type = type;
    return ptr;
}

void destroy_event(event_t *e)
{
    free(e);
}

void swap(event_t **a, event_t **b)
{
    event_t *c = *a;
    *a = *b;
    *b = c;
}

void sort(event_t *t[], int n)
{
    for (int i = 0; i < n; i++)
          for (int j = 0; j < n-1-i; j++)
              if (t[j]->time > t[j+1]->time)
                  swap(&t[j], &t[j+1]);
}

void shift(event_t **t, int n)
{
    for (int i = 0; i < n-1; i++)
        t[i] = t[i+1];
}
