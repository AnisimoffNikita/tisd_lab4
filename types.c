#include "types.h"

#include <stdlib.h>
#include <malloc.h>

double rand_0_1()
{
    return (double)rand()/(double)RAND_MAX;
}

application_t *create_application(application_type_t type, float t1, float t2, float t3, float t4)
{
    application_t *ptr = malloc(sizeof(application_t));
    ptr->type = type;
    ptr->add_time = (t2 - t1)*rand_0_1() + t1;
    ptr->process_time = (t4 - t3)*rand_0_1() + t3;
    return ptr;
}

void destroy_application(application_t *a)
{
    free(a);
}
