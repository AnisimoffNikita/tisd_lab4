#include "defines.h"

#include <stdio.h>

void print_error(int e)
{
    switch (e) {
    case EMPTY_QUEUE:
        printf("queue empty\n");
        break;
    case FULL_QUEUE:
        printf("queue full\n");
        break;
    case UNEXPECTD_ERROR:
        printf("unexpected error\n");
        break;
    default:
        break;
    }
}
