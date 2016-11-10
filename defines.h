#ifndef QUEUE_ERRORS
#define QUEUE_ERRORS

#define OK 0
#define EMPTY_QUEUE -1
#define FULL_QUEUE -2
#define UNEXPECTD_ERROR -3

void print_error(int e);

typedef enum
{
    LIST,
    ARRAY
} queue_type;

#endif // QUEUE_ERRORS

