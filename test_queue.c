#include "test_queue.h"

#include "queue_array.h"
#include "queue_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void test_queue(int type)
{
    if (type == LIST)
        test_queue_list();
    else
        test_queue_array();
}

int choice_test()
{
    printf("What do you want to do?\n");
    printf("1 - enqueue\n");
    printf("2 - dequeue\n");
    printf("3 - watch\n");
    printf("0 - stop\n");
    printf("> ");
    char buffer[10];
    scanf("%s", buffer);
    int c = atoi(buffer);
    while (c < 0 || c > 3) {
        printf("Repeat input> ");
        scanf("%s", buffer);
        c = atoi(buffer);
    }
    return c;
}

void test_queue_list_dequeue(queue_l *s);
void test_queue_list_enqueue(queue_l *s);
void test_queue_list_watch(queue_l *s);

void test_queue_list()
{
    queue_l *s = create_queue_l();
    int stop = 0;
    while (!stop) {
        int c = choice_test();
        switch(c)
        {
        case 0:
            stop = 1;
            break;
        case 1:
            test_queue_list_enqueue(s);
            break;
        case 2:
            test_queue_list_dequeue(s);
            break;
        case 3:
            test_queue_list_watch(s);
            break;
        default:
            break;
        }
    }
    destroy_queue_l(s);
}

void test_queue_list_enqueue(queue_l *s)
{
    printf("Pushing data...\n");
    int ec;
    printf("Input number: ");
    char buffer[10];
    scanf("%s", buffer);
    float num = atof(buffer);
    application_t *a = create_application(TYPE_1,0,0,0,0,0);
    a->add_time = num;
    enqueue_l(s, a, &ec);
    if (ec != OK)
        print_error(ec);
    else
        printf("OK\n");
}

void test_queue_list_dequeue(queue_l *s)
{
    printf("Popping data...\n");
    int ec;
    application_t *a = dequeue_l(s, &ec);
    if (ec != OK)
        print_error(ec);
    else
    {
        destroy_application(a);
        printf("OK\n");
    }
}

void test_queue_list_watch(queue_l *s)
{
    print_dump_queue_list("", s);
}



void test_queue_array_dequeue(queue_a *s);
void test_queue_array_enqueue(queue_a *s);
void test_queue_array_watch(queue_a *s);
void test_queue_array_clear(queue_a *s);

void test_queue_array()
{
    queue_a *s = create_queue_a();
    int stop = 0;
    while (!stop) {
        int c = choice_test();
        switch(c)
        {
        case 0:
            stop = 1;
            break;
        case 1:
            test_queue_array_enqueue(s);
            break;
        case 2:
            test_queue_array_dequeue(s);
            break;
        case 3:
            test_queue_array_watch(s);
            break;
        default:
            break;
        }
    }
    destroy_queue_a(s);
}

void test_queue_array_enqueue(queue_a *s)
{
    printf("Pushing data...\n");
    int ec;
    printf("Input number: ");
    char buffer[10];
    scanf("%s", buffer);
    float num = atof(buffer);
    application_t *a = create_application(TYPE_1,0,0,0,0,0);
    a->add_time = num;
    enqueue_a(s, a, &ec);
    if (ec != OK)
        print_error(ec);
    else
        printf("OK\n");
}

void test_queue_array_dequeue(queue_a *s)
{
    printf("Popping data...\n");
    int ec;
    application_t *a = dequeue_a(s, &ec);
    if (ec != OK)
        print_error(ec);
    else
    {
        destroy_application(a);
        printf("OK\n");
    }
}

void test_queue_array_watch(queue_a *s)
{
    print_dump_queue_array("", s);
}
