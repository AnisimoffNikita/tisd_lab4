#ifndef PROCESS_H
#define PROCESS_H

typedef enum
{
    LIST,
    ARRAY
} queue_type;

void service_unit(float *t, int type);

void service_unit_list(float *t);
void service_unit_array(float *t);

#endif // PROCESS_H
