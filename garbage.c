#include "garbage.h"

#include <stdio.h>

object_t *freed[FREED_SIZE];
int freed_counter = 0;

void dump_freed(object_t *s)
{
    if (freed_counter < FREED_SIZE)
        freed[freed_counter++] = s;
    else
    {
        for (int i = 0; i < FREED_SIZE - 1; i++)
            freed[i] = freed[i+1];
        freed[freed_counter - 1] = s;
    }
}

void delete_freed(object_t *s)
{
    int f = 0;
    while (f < FREED_SIZE && freed[f] != s)
        f++;
    if (f < FREED_SIZE)
    {
        for (int i = f; i < freed_counter - 1; i++)
            freed[i] = freed[i+1];
        freed_counter--;
    }
}

void print_freed()
{
    printf("DELETED DUMP\n");
    if (freed_counter)
    {
        printf("%16s\n", "POINTER");
        int begin = freed_counter < FREED_SIZE ? 0 : freed_counter - FREED_SIZE + 1;
        for (int i = begin; i < freed_counter; i++)
        {
            printf("%16p\n", freed[i % FREED_SIZE]);
        }
    }
    else
    {
        printf("%16s\n", "no deletions");
    }
}

void clear_freed()
{
    freed_counter = 0;
}
