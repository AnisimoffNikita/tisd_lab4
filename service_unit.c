#include "service_unit.h"

#include "queue_array.h"
#include "queue_list.h"

#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define SIZE 1000

void service_unit(float *t, int type)
{
    if (type == LIST)
        service_unit_list(t);
    else
        service_unit_array(t);
};



enum
{
    NONE,
    ENQUEUE,
    PROCESS_END,
};

void print_info(int q1_p, int q1_e, float q1_a, int q1_l, int q2_p, int q2_e, float q2_a, int q2_l)
{
    printf("*********************************\n");
    printf("q1: processed: %d\n", q1_p);
    printf("q1: applications entered in queue: %d\n", q1_e);
    printf("q1: len: %d\n", q1_l);
    printf("q1: average time in queue: %.2f\n", q1_a);
    printf("q2: processed: %d\n", q2_p);
    printf("q2: applications entered in queue: %d\n", q2_e);
    printf("q2: len: %d\n", q2_l);
    printf("q2: average time in queue: %.2f\n", q2_a);
}

void service_unit_list(float *t)
{
    int code;
    queue_l *q1 = create_queue_l();
    queue_l *q2 = create_queue_l();

    float q1_add_time = 0;
    float q1_process_time = 0;
    int q1_applications = 0;
    int q1_dequeues = 0;
    int q1_processed = 0;
    float q1_wait_time = 0;

    float q2_add_time = 0;
    float q2_process_time = 0;
    int q2_applications = 0;
    int q2_enqueues = 0;
    int q2_dequeues = 0;
    int q2_processed = 0;
    float q2_wait_time = 0;

    event_t *events[3] = {NULL};
    event_t *current_event;
    int events_size = 0;

    application_t *current = NULL;
    float current_begin = 0;

    application_t *a = create_application(TYPE_1, t[0], t[1], t[2], t[3], 0);
    events[events_size] = create_event(a, a->add_time, ENQUEUE);
    events_size++;

    a = create_application(TYPE_2, t[4], t[5], t[6], t[7], 0);
    events[events_size] = create_event(a, a->add_time, ENQUEUE);
    events_size++;

    clock_t b = clock();

    while(q1_processed < SIZE)
    {
        sort(events, events_size);
        current_event = events[0];
        float current_time = current_event->time;
        switch(current_event->type)
        {
        case ENQUEUE:
            if (current_event->a->type == TYPE_1)
            {
                q1_applications++;
                q1_add_time += current_event->a->add_time;
                enqueue_l(q1, current_event->a, &code);
                destroy_event(current_event);
                shift(events, events_size);
                events_size--;
                if (q1_applications < SIZE)
                {
                    a = create_application(TYPE_1, t[0], t[1], t[2], t[3], current_time);
                    events[events_size] = create_event(a, current_time + a->add_time, ENQUEUE);
                    events_size++;
                }
                if (!current || current->type == TYPE_2)
                {
                    if (current && current->type == TYPE_2)
                    {
                        if (events[0]->a == current)
                        {
                            destroy_event(events[0]);
                            shift(events, events_size);
                            q2_process_time += current_time - current_begin;
                        }
                        else if (events[1]->a == current)
                        {
                            destroy_event(events[1]);
                            events[1] = events[2];
                        }
                        events_size--;
                        q2_enqueues++;
                        enqueue_l(q2, current, &code);
                    }
                    current = dequeue_l(q1, &code);
                    q1_dequeues++;
                    q1_wait_time += current_time - current->start_time;
                    event_t *e = create_event(current, current_time + current->process_time, PROCESS_END);
                    current_begin = current_time;
                    events[events_size] = e;
                    events_size++;
                }
            }
            else
            {
                q2_applications++;
                q2_enqueues++;
                q2_add_time += current_event->a->add_time;
                enqueue_l(q2, current_event->a, &code);
                destroy_event(current_event);
                shift(events, events_size);
                events_size--;
                a = create_application(TYPE_2, t[4], t[5], t[6], t[7], current_time);
                events[events_size] = create_event(a, current_time + a->add_time, ENQUEUE);
                events_size++;
                if (!current)
                {
                    current = dequeue_l(q2, &code);
                    q2_dequeues++;
                    q2_wait_time += current_time - current->start_time;
                    event_t *e = create_event(current, current_time + current->process_time, PROCESS_END);
                    current_begin = current_time;
                    events[events_size] = e;
                    events_size++;
                }
            }
            break;
        case PROCESS_END:
            if (current->type == TYPE_1)
            {
                q1_process_time += current->process_time;
                q1_processed++;
                if (q1_processed % 100 == 0)
                {
                    print_info(q1_processed, q1_applications, q1->size, (float)q1_wait_time/q1_dequeues,
                               q2_processed, q2_applications, (float)q2_wait_time/q2_dequeues, q2->size);
                }
            }
            else
            {
                q2_process_time += current->process_time;
                q2_processed++;
            }
            destroy_application(current);
            current = NULL;
            destroy_event(current_event);
            shift(events, events_size);
            events_size--;
            if (!is_empty_l(q1))
            {
                current = dequeue_l(q1, &code);
                q1_dequeues++;
                q1_wait_time += current_time - current->start_time;
                event_t *e = create_event(current, current_time + current->process_time, PROCESS_END);
                current_begin = current_time;
                events[events_size] = e;
                events_size++;
            }
            else if (!is_empty_l(q2))
            {
                current = dequeue_l(q2, &code);
                q2_dequeues++;
                q2_wait_time += current_time - current->start_time;
                event_t *e = create_event(current, current_time + current->process_time, PROCESS_END);
                current_begin = current_time;
                events[events_size] = e;
                events_size++;
            }
            break;
        }
    }

    clock_t r_time = clock() - b;

    float tmp;
    printf("****************Q1***************\n");
    printf("Applications: %d\n", q1_applications);
    printf("Processed: %d\n", q1_processed);
    tmp = -q1_process_time + q1_add_time;
    if (tmp < 0) tmp = 0;
    printf("Input check\n");
    tmp = q1_add_time / ((t[1]+t[0])/2.0);
    printf("Expected number of applications: %.2f\n", tmp);
    printf("Result number of applications: %d\n", q1_applications);
    printf("Error: %.2f%%\n", 100.0*abs(q1_applications - tmp)/tmp);
    printf("Output check\n");
    tmp = SIZE * ((t[3]+t[2])/2.0);
    printf("Expected process time: %.2f\n", tmp);
    printf("Result process time: %.2f\n", q1_process_time);
    printf("Error: %.2f%%\n", 100.0*abs(q1_process_time - tmp)/tmp);

    printf("****************Q2***************\n");
    printf("Applications: %d\n", q2_applications);
    printf("Removed from OA: %d\n", q2_enqueues - q2_applications);
    printf("Processed: %d\n", q2_processed);
    tmp = -q2_process_time + q2_add_time;
    if (tmp < 0) tmp = 0;
    printf("Input check\n");
    tmp = q2_add_time / ((t[5]+t[4])/2.0);
    printf("Expected number of applications: %.2f\n", tmp);
    printf("Result number of applications: %d\n", q2_applications);
    printf("Error: %.2f%%\n", 100.0*abs(q2_applications - tmp)/tmp);
    printf("Output check\n");
    tmp = q2_processed*(t[7]+t[6])/2.0 + (q2_applications - q2_processed) * (t[7]+t[6])/4.0;
    printf("Expected process time: %.2f\n", tmp);
    printf("Result process time: %.2f\n", q2_process_time);
    printf("Error: %.2f%%\n", 100.0*abs(q2_process_time - tmp)/tmp);

    printf("**************TOTAL***************\n");
    printf("Total modeling time: %.2f\n", q1_add_time);
    printf("Input check\n");
    tmp = q1_add_time / ((t[1]+t[0])/2.0) + q2_add_time / ((t[5]+t[4])/2.0);
    printf("Expected number of applications: %.2f\n", tmp);
    printf("Result number of applications: %d\n", q1_applications+q2_applications);
    printf("Error: %.2f%%\n", 100.0*abs(q1_applications + q2_applications - tmp)/tmp);
    printf("Output check\n");
    tmp = SIZE * ((t[3]+t[2])/2.0) + q2_processed*(t[7]+t[6])/2.0 + (q2_applications - q2_processed) * (t[7]+t[6])/4.0;
    printf("Expected process time: %.2f\n", tmp);
    printf("Result process time: %.2f\n", q1_process_time + q2_process_time);
    printf("Error: %.2f%%\n", 100.0*abs(q1_process_time + q2_process_time - tmp)/tmp);
    printf("*********************************\n");

    printf("Real working time: %.5f\n", (double)(r_time)/CLOCKS_PER_SEC);

    destroy_queue_l(q1);
    destroy_queue_l(q2);
}


void service_unit_array(float *t)
{
    int code;
    queue_a *q1 = create_queue_a();
    queue_a *q2 = create_queue_a();

    float q1_add_time = 0;
    float q1_process_time = 0;
    int q1_applications = 0;
    int q1_dequeues = 0;
    int q1_processed = 0;
    float q1_wait_time = 0;

    float q2_add_time = 0;
    float q2_process_time = 0;
    int q2_applications = 0;
    int q2_enqueues = 0;
    int q2_dequeues = 0;
    int q2_processed = 0;
    float q2_wait_time = 0;

    event_t *events[3] = {NULL};
    event_t *current_event;
    int events_size = 0;

    application_t *current = NULL;
    float current_begin = 0;

    application_t *a = create_application(TYPE_1, t[0], t[1], t[2], t[3], 0);
    events[events_size] = create_event(a, a->add_time, ENQUEUE);
    events_size++;

    a = create_application(TYPE_2, t[4], t[5], t[6], t[7], 0);
    events[events_size] = create_event(a, a->add_time, ENQUEUE);
    events_size++;

    clock_t b = clock();

    while(q1_processed < SIZE)
    {
        sort(events, events_size);
        current_event = events[0];
        float current_time = current_event->time;
        switch(current_event->type)
        {
        case ENQUEUE:
            if (current_event->a->type == TYPE_1)
            {
                q1_applications++;
                q1_add_time += current_event->a->add_time;
                enqueue_a(q1, current_event->a, &code);
                destroy_event(current_event);
                shift(events, events_size);
                events_size--;
                if (q1_applications < SIZE)
                {
                    a = create_application(TYPE_1, t[0], t[1], t[2], t[3], current_time);
                    events[events_size] = create_event(a, current_time + a->add_time, ENQUEUE);
                    events_size++;
                }
                if (!current || current->type == TYPE_2)
                {
                    if (current && current->type == TYPE_2)
                    {
                        if (events[0]->a == current)
                        {
                            destroy_event(events[0]);
                            shift(events, events_size);
                            q2_process_time += current_time - current_begin;
                        }
                        else if (events[1]->a == current)
                        {
                            destroy_event(events[1]);
                            events[1] = events[2];
                        }
                        events_size--;
                        q2_enqueues++;
                        enqueue_a(q2, current, &code);
                    }
                    current = dequeue_a(q1, &code);
                    q1_dequeues++;
                    q1_wait_time += current_time - current->start_time;
                    event_t *e = create_event(current, current_time + current->process_time, PROCESS_END);
                    current_begin = current_time;
                    events[events_size] = e;
                    events_size++;
                }
            }
            else
            {
                q2_applications++;
                q2_enqueues++;
                q2_add_time += current_event->a->add_time;
                enqueue_a(q2, current_event->a, &code);
                destroy_event(current_event);
                shift(events, events_size);
                events_size--;
                a = create_application(TYPE_2, t[4], t[5], t[6], t[7], current_time);
                events[events_size] = create_event(a, current_time + a->add_time, ENQUEUE);
                events_size++;
                if (!current)
                {
                    current = dequeue_a(q2, &code);
                    q2_dequeues++;
                    q2_wait_time += current_time - current->start_time;
                    event_t *e = create_event(current, current_time + current->process_time, PROCESS_END);
                    current_begin = current_time;
                    events[events_size] = e;
                    events_size++;
                }
            }
            break;
        case PROCESS_END:
            if (current->type == TYPE_1)
            {
                q1_process_time += current->process_time;
                q1_processed++;
                if (q1_processed % 100 == 0)
                {
                    print_info(q1_processed, q1_applications, q1->size, (float)q1_wait_time/q1_dequeues,
                               q2_processed, q2_applications, (float)q2_wait_time/q2_dequeues, q2->size);
                }
            }
            else
            {
                q2_process_time += current->process_time;
                q2_processed++;
            }
            destroy_application(current);
            current = NULL;
            destroy_event(current_event);
            shift(events, events_size);
            events_size--;
            if (!is_empty_a(q1))
            {
                current = dequeue_a(q1, &code);
                q1_dequeues++;
                q1_wait_time += current_time - current->start_time;
                event_t *e = create_event(current, current_time + current->process_time, PROCESS_END);
                current_begin = current_time;
                events[events_size] = e;
                events_size++;
            }
            else if (!is_empty_a(q2))
            {
                current = dequeue_a(q2, &code);
                q2_dequeues++;
                q2_wait_time += current_time - current->start_time;
                event_t *e = create_event(current, current_time + current->process_time, PROCESS_END);
                current_begin = current_time;
                events[events_size] = e;
                events_size++;
            }
            break;
        }
    }

    clock_t r_time = clock() - b;

    float tmp;
    printf("****************Q1***************\n");
    printf("Applications: %d\n", q1_applications);
    printf("Processed: %d\n", q1_processed);
    tmp = -q1_process_time + q1_add_time;
    if (tmp < 0) tmp = 0;
    printf("Input check\n");
    tmp = q1_add_time / ((t[1]+t[0])/2.0);
    printf("Expected number of applications: %.2f\n", tmp);
    printf("Result number of applications: %d\n", q1_applications);
    printf("Error: %.2f%%\n", 100.0*abs(q1_applications - tmp)/tmp);
    printf("Output check\n");
    tmp = SIZE * ((t[3]+t[2])/2.0);
    printf("Expected process time: %.2f\n", tmp);
    printf("Result process time: %.2f\n", q1_process_time);
    printf("Error: %.2f%%\n", 100.0*abs(q1_process_time - tmp)/tmp);

    printf("****************Q2***************\n");
    printf("Applications: %d\n", q2_applications);
    printf("Removed from OA: %d\n", q2_enqueues - q2_applications);
    printf("Processed: %d\n", q2_processed);
    tmp = -q2_process_time + q2_add_time;
    if (tmp < 0) tmp = 0;
    printf("Input check\n");
    tmp = q2_add_time / ((t[5]+t[4])/2.0);
    printf("Expected number of applications: %.2f\n", tmp);
    printf("Result number of applications: %d\n", q2_applications);
    printf("Error: %.2f%%\n", 100.0*abs(q2_applications - tmp)/tmp);
    printf("Output check\n");
    tmp = q2_processed*(t[7]+t[6])/2.0 + (q2_applications - q2_processed) * (t[7]+t[6])/4.0;
    printf("Expected process time: %.2f\n", tmp);
    printf("Result process time: %.2f\n", q2_process_time);
    printf("Error: %.2f%%\n", 100.0*abs(q2_process_time - tmp)/tmp);

    printf("**************TOTAL***************\n");
    printf("Total modeling time: %.2f\n", q1_add_time);
    printf("Input check\n");
    tmp = q1_add_time / ((t[1]+t[0])/2.0) + q2_add_time / ((t[5]+t[4])/2.0);
    printf("Expected number of applications: %.2f\n", tmp);
    printf("Result number of applications: %d\n", q1_applications+q2_applications);
    printf("Error: %.2f%%\n", 100.0*abs(q1_applications + q2_applications - tmp)/tmp);
    printf("Output check\n");
    tmp = SIZE * ((t[3]+t[2])/2.0) + q2_processed*(t[7]+t[6])/2.0 + (q2_applications - q2_processed) * (t[7]+t[6])/4.0;
    printf("Expected process time: %.2f\n", tmp);
    printf("Result process time: %.2f\n", q1_process_time + q2_process_time);
    printf("Error: %.2f%%\n", 100.0*abs(q1_process_time + q2_process_time - tmp)/tmp);
    printf("*********************************\n");

    printf("Real working time: %.5f\n", (double)(r_time)/CLOCKS_PER_SEC);

    destroy_queue_a(q1);
    destroy_queue_a(q2);
}
