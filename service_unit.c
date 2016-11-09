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
    //else
        //process_array(t);
};

void service_unit_list(float *t)
{
    int code;
    queue_l *q1 = create_queue_l();
    queue_l *q2 = create_queue_l();

    float q1_busy_time = 0;
    float q2_busy_time = 0;
    application_t *work_q2;
    int j = 0;
    int adds_q2 = 0;
    int proceseed_q2 = 0;

    float type1_add_time = 0;
    float type2_add_time = 0;
    float type1_process_time = 0;
    float type2_process_time = 0;

    float q2_begin_add = 0;

    for (int i = 0; i < SIZE; i++)
    {
        if (!is_empty_l(q1) && q1_busy_time == 0)
        {
            if (q2_busy_time != 0)
            {
                type2_process_time -=  q2_busy_time;
                enqueue_l(q2, work_q2, &code);
                work_q2 = NULL;
                adds_q2++;
                q2_busy_time = 0;
            }
            application_t *t = dequeue_l(q1, &code);
            q1_busy_time = t->process_time;
            type1_process_time += t->process_time;
            destroy_application(t);
        }

        application_t *a1 = create_application(TYPE_1, t[0], t[1], t[2], t[3]);
        enqueue_l(q1, a1, &code);
        type1_add_time += a1->add_time;


        float iter_time = q2_begin_add;
        float q2_begin_process = 0;
        while (iter_time < a1->add_time && j < SIZE)
        {
            application_t *a2 = create_application(TYPE_2, t[4], t[5], t[6], t[7]);
            if (is_empty_l(q2))
                q2_begin_process = a2->add_time;
            enqueue_l(q2, a2, &code);
            j++;
            adds_q2++;
            type2_add_time += a2->add_time;
            iter_time += a2->add_time;
        }
        q2_begin_add = fmod(iter_time, a1->process_time);

        if (a1->add_time >= q1_busy_time + q2_begin_process)
        {
            iter_time = a1->add_time - q1_busy_time - q2_begin_process;
            work_q2 = NULL;
            while (!is_empty_l(q2) && iter_time < a1->add_time)
            {
                if (work_q2)
                {
                    proceseed_q2++;
                    destroy_application(work_q2);
                }
                work_q2 = dequeue_l(q2, &code);
                q2_busy_time = work_q2->process_time;
                type2_process_time += work_q2->process_time;
                iter_time += work_q2->process_time;
            }
            q2_busy_time = iter_time - a1->add_time;
            if (q2_busy_time < 0 && work_q2)
            {
                proceseed_q2++;
                destroy_application(work_q2);
                q2_busy_time = 0;
            }
            if (!work_q2) q2_busy_time = 0;
            q1_busy_time = 0;
        }
        else
        {
            q1_busy_time = a1->add_time - q1_busy_time;
        }
    }
    while (!is_empty_l(q1))
    {
        application_t *a1 = dequeue_l(q1, &code);
        type1_process_time += a1->process_time;
    }
    while (!is_empty_l(q2))
    {
        application_t *a2 = dequeue_l(q2, &code);
        type2_process_time += a2->process_time;
    }

    printf("Expected q1 add time: %.2f\n", SIZE*(t[1]+t[0])/2.0);
    printf("Result q1 add time: %.2f\n", type1_add_time);
    printf("Expected q1 process time: %.2f\n", SIZE*(t[3]+t[2])/2.0);
    printf("Result q1 process time: %.2f\n", type1_process_time);
    printf("Expected q2 add time: %.2f\n", SIZE*(t[5]+t[4])/2.0);
    printf("Result q2 add time: %.2f\n", type2_add_time);
    printf("Expected q2 process time: %.2f\n", SIZE*(t[7]+t[6])/2.0 + (adds_q2-SIZE) * (t[7]+t[6])/4.0);
    printf("Result q2 process time: %.2f\n", type2_process_time);
}
