#ifndef TYPES_H
#define TYPES_H

typedef enum {
    TYPE_1,
    TYPE_2
} application_type_t;

typedef struct {
    application_type_t type;
    float add_time;
    float process_time;
    float start_time;
} application_t;

application_t *create_application(application_type_t type, float t1, float t2, float t3, float t4, float start_t);
void destroy_application(application_t *a);

typedef struct
{
    application_t *a;
    float time;
    int type;
} event_t;

event_t *create_event(application_t *a, float time, float type);
void destroy_event(event_t *e);

void sort(event_t *t[], int n);
void shift(event_t **t, int n);
#endif // TYPES_H
