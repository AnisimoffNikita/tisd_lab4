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
} application_t;

application_t *create_application(application_type_t type, float t1, float t2, float t3, float t4);
void destroy_application(application_t *a);

#endif // TYPES_H
