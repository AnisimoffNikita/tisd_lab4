#include "service_unit.h"

#include "queue_array.h"
#include "queue_list.h"

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

void print_info(int i, int q1_e, int q1_d, float q1_a, int q2_e, int q2_d, float q2_a)
{
    printf("*********************************\n");
    printf("q1: processed: %d\n", i);
    printf("q1: enqueues: %d\n", q1_e);
    printf("q1: dequeues: %d\n", q1_d);
    printf("q1: average time in queue: %.2f\n", q1_a);
    printf("q2: enqueues: %d\n", q2_e);
    printf("q2: dequeues: %d\n", q2_d);
    printf("q2: average time in queue: %.2f\n", q2_a);
}

void service_unit_list(float *t)
{
    int code;
    queue_l *q1 = create_queue_l();
    queue_l *q2 = create_queue_l();

    float q1_add_time = 0;
    float q1_process_time = 0;
    int q1_inserts = 0;
    int q1_dequeues = 0;
    int q1_processed = 0;
    float q1_wait_time = 0;

    float q2_add_time = 0;
    float q2_process_time = 0;
    int q2_inserts = 0;
    int q2_added = 0;
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

    while(q1_processed < SIZE || q2_processed < SIZE)
    {
        sort(events, events_size);
        current_event = events[0];
        float current_time = current_event->time;
        switch(current_event->type)
        {
        case ENQUEUE:
            if (current_event->a->type == TYPE_1)
            {
                q1_inserts++;
                q1_add_time += current_event->a->add_time;
                enqueue_l(q1, current_event->a, &code);
                destroy_event(current_event);
                shift(events, events_size);
                events_size--;
                if (q1_inserts < SIZE)
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
                        q2_added++;
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
                q2_inserts++;
                q2_added++;
                q2_add_time += current_event->a->add_time;
                enqueue_l(q2, current_event->a, &code);
                destroy_event(current_event);
                shift(events, events_size);
                events_size--;
                if (q2_inserts < SIZE)
                {
                    a = create_application(TYPE_2, t[4], t[5], t[6], t[7], current_time);
                    events[events_size] = create_event(a, current_time + a->add_time, ENQUEUE);
                    events_size++;
                }
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
                    print_info(q1_processed, q1_inserts, q1_dequeues, (float)q1_wait_time/q1_dequeues,
                               q2_inserts, q2_added, (float)q2_wait_time/q2_dequeues);
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
    float tmp;
    printf("****************Q1***************\n");
    printf("Enqueues: %d\n", q1_inserts);
    printf("Dequeues: %d\n", q1_dequeues);
    tmp = q1_process_time - q1_add_time;
    if (tmp < 0) tmp = 0;
    printf("Idle time: %.2f\n", tmp);
    printf("Input check\n");
    tmp = q1_add_time / ((t[1]+t[0])/2.0);
    printf("Expected number of applications: %.2f\n", tmp);
    printf("Result number of applications: %d\n", q1_inserts);
    printf("Error: %.2f\n", 100.0*abs(q1_inserts - tmp)/tmp);
    printf("Output check\n");
    tmp = SIZE * ((t[3]+t[2])/2.0);
    printf("Expected modeling time: %.2f\n", tmp);
    printf("Result modeling time: %.2f\n", q1_process_time);
    printf("Error: %.2f\n", 100.0*abs(q1_process_time - tmp)/tmp);

    printf("****************Q2***************\n");
    printf("Enqueues: %d\n", q2_added);
    printf("Dequeues: %d\n", q2_dequeues);
    tmp = q2_process_time - q2_add_time;
    if (tmp < 0) tmp = 0;
    printf("Idle time: %.2f\n", tmp);
    printf("Input check\n");
    tmp = q2_add_time / ((t[5]+t[4])/2.0);
    printf("Expected number of applications: %.2f\n", tmp);
    printf("Result number of applications: %d\n", q2_inserts);
    printf("Error: %.2f\n", 100.0*abs(q2_inserts - tmp)/tmp);
    printf("Output check\n");
    tmp = SIZE*(t[7]+t[6])/2.0 + (q2_added-SIZE) * (t[7]+t[6])/4.0;
    printf("Expected modeling time: %.2f\n", tmp);
    printf("Result modeling time: %.2f\n", q2_process_time);
    printf("Error: %.2f\n", 100.0*abs(q2_process_time - tmp)/tmp);

    printf("**************TOTAL***************\n");
    printf("Enqueues: %d\n", q1_inserts + q2_added);
    printf("Dequeues: %d\n", q1_dequeues + q2_dequeues);
    tmp = q1_process_time + q2_process_time - q1_add_time - q2_add_time;
    if (tmp < 0) tmp = 0;
    printf("Total idle time: %.2f\n", tmp);
    printf("Input check\n");
    tmp = (q1_add_time + q2_add_time) / ( (t[5]+t[4])/4.0 + ((t[1]+t[0])/4.0) );
    printf("Expected number of applications: %.2f\n", tmp);
    printf("Result number of applications: %d\n", q1_inserts+q2_inserts);
    printf("Error: %.2f\n", 100.0*abs(q1_inserts + q2_inserts - tmp)/tmp);
    printf("Output check\n");
    tmp = SIZE*(t[3]+t[2])/2.0 + SIZE*(t[7]+t[6])/2.0 + (q2_added-SIZE) * (t[7]+t[6])/4.0;
    printf("Expected modeling time: %.2f\n", tmp);
    printf("Result modeling time: %.2f\n", q1_process_time + q2_process_time);
    printf("Error: %.2f\n", 100.0*abs(q1_process_time + q2_process_time - tmp)/tmp);
    printf("*********************************\n");
}


void service_unit_array(float *t)
{
    int code;
    queue_a *q1 = create_queue_a();
    queue_a *q2 = create_queue_a();

    float q1_add_time = 0;
    float q1_process_time = 0;
    int q1_inserts = 0;
    int q1_dequeues = 0;
    int q1_processed = 0;
    float q1_wait_time = 0;

    float q2_add_time = 0;
    float q2_process_time = 0;
    int q2_inserts = 0;
    int q2_added = 0;
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

    while(q1_processed < SIZE || q2_processed < SIZE)
    {
        sort(events, events_size);
        current_event = events[0];
        float current_time = current_event->time;
        switch(current_event->type)
        {
        case ENQUEUE:
            if (current_event->a->type == TYPE_1)
            {
                q1_inserts++;
                q1_add_time += current_event->a->add_time;
                enqueue_a(q1, current_event->a, &code);
                destroy_event(current_event);
                shift(events, events_size);
                events_size--;
                if (q1_inserts < SIZE)
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
                        q2_added++;
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
                q2_inserts++;
                q2_added++;
                q2_add_time += current_event->a->add_time;
                enqueue_a(q2, current_event->a, &code);
                destroy_event(current_event);
                shift(events, events_size);
                events_size--;
                if (q2_inserts < SIZE)
                {
                    a = create_application(TYPE_2, t[4], t[5], t[6], t[7], current_time);
                    events[events_size] = create_event(a, current_time + a->add_time, ENQUEUE);
                    events_size++;
                }
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
                    print_info(q1_processed, q1_inserts, q1_dequeues, (float)q1_wait_time/q1_dequeues,
                               q2_inserts, q2_added, (float)q2_wait_time/q2_dequeues);
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
    float tmp;
    printf("****************Q1***************\n");
    printf("Enqueues: %d\n", q1_inserts);
    printf("Dequeues: %d\n", q1_dequeues);
    tmp = q1_process_time - q1_add_time;
    if (tmp < 0) tmp = 0;
    printf("Idle time: %.2f\n", tmp);
    printf("Input check\n");
    tmp = q1_add_time / ((t[1]+t[0])/2.0);
    printf("Expected number of applications: %.2f\n", tmp);
    printf("Result number of applications: %d\n", q1_inserts);
    printf("Error: %.2f\n", 100.0*abs(q1_inserts - tmp)/tmp);
    printf("Output check\n");
    tmp = SIZE * ((t[3]+t[2])/2.0);
    printf("Expected modeling time: %.2f\n", tmp);
    printf("Result modeling time: %.2f\n", q1_process_time);
    printf("Error: %.2f\n", 100.0*abs(q1_process_time - tmp)/tmp);

    printf("****************Q2***************\n");
    printf("Enqueues: %d\n", q2_added);
    printf("Dequeues: %d\n", q2_dequeues);
    tmp = q2_process_time - q2_add_time;
    if (tmp < 0) tmp = 0;
    printf("Idle time: %.2f\n", tmp);
    printf("Input check\n");
    tmp = q2_add_time / ((t[5]+t[4])/2.0);
    printf("Expected number of applications: %.2f\n", tmp);
    printf("Result number of applications: %d\n", q2_inserts);
    printf("Error: %.2f\n", 100.0*abs(q2_inserts - tmp)/tmp);
    printf("Output check\n");
    tmp = SIZE*(t[7]+t[6])/2.0 + (q2_added-SIZE) * (t[7]+t[6])/4.0;
    printf("Expected modeling time: %.2f\n", tmp);
    printf("Result modeling time: %.2f\n", q2_process_time);
    printf("Error: %.2f\n", 100.0*abs(q2_process_time - tmp)/tmp);

    printf("**************TOTAL***************\n");
    printf("Enqueues: %d\n", q1_inserts + q2_added);
    printf("Dequeues: %d\n", q1_dequeues + q2_dequeues);
    tmp = q1_process_time + q2_process_time - q1_add_time - q2_add_time;
    if (tmp < 0) tmp = 0;
    printf("Total idle time: %.2f\n", tmp);
    printf("Input check\n");
    tmp = (q1_add_time + q2_add_time) / ( (t[5]+t[4])/4.0 + ((t[1]+t[0])/4.0) );
    printf("Expected number of applications: %.2f\n", tmp);
    printf("Result number of applications: %d\n", q1_inserts+q2_inserts);
    printf("Error: %.2f\n", 100.0*abs(q1_inserts + q2_inserts - tmp)/tmp);
    printf("Output check\n");
    tmp = SIZE*(t[3]+t[2])/2.0 + SIZE*(t[7]+t[6])/2.0 + (q2_added-SIZE) * (t[7]+t[6])/4.0;
    printf("Expected modeling time: %.2f\n", tmp);
    printf("Result modeling time: %.2f\n", q1_process_time + q2_process_time);
    printf("Error: %.2f\n", 100.0*abs(q1_process_time + q2_process_time - tmp)/tmp);
    printf("*********************************\n");
}
