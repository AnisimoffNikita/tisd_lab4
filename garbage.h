#ifndef GARBAGE_H
#define GARBAGE_H

#define FREED_SIZE 32

#include "queue_list.h"

void dump_freed(object_t *s);
void print_freed();
void clear_freed();
void delete_freed(object_t *s);

#endif // GARBAGE_H
