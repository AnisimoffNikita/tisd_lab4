#include <stdio.h>

#include "stdlib.h"
#include "service_unit.h"
#include "defines.h"
#include "test_queue.h"

int choice_queue();
int choice_action();

int main(void)
{
    float t[8];
    float dt[8] = {1, 5, 0, 4, 1, 3, 0, 1};
    int stop = 0;
    while (!stop) {
        int queue_type = choice_queue();

        int c = choice_action();
        switch(c)
        {
        case 0:
            stop = 1;
            break;
        case 1:
            printf("Input bounds of 1st type addition time (a b): ");
            scanf("%f %f", &t[0], &t[1]);
            printf("Input bounds of 1st type process time (a b): ");
            scanf("%f %f", &t[2], &t[3]);
            printf("Input bounds of 2st type addition time (a b): ");
            scanf("%f %f", &t[4], &t[5]);
            printf("Input bounds of 2st type process time (a b): ");
            scanf("%f %f", &t[6], &t[7]);
            service_unit(t, queue_type);
            break;
        case 2:
            service_unit(dt, LIST);
            break;
        case 3:
            test_queue(queue_type);
            break;
        default:
            break;
        }
    }
    return 0;
}

int choice_queue()
{
    printf("What type of queue do you want to use?\n");
    printf("1 - list based\n");
    printf("2 - array based\n");
    printf("> ");
    char buffer[10];
    scanf("%s", buffer);
    int c = atoi(buffer) - 1;
    while (c < 0 || c > 1) {
        printf("Repeat input> ");
        scanf("%s", buffer);
        c = atoi(buffer);
    }
    return c;
}

int choice_action()
{
    printf("What do you want to do?\n");
    printf("1 - model service unit\n");
    printf("2 - model service unit with default data\n");
    printf("3 - test queue\n");
    printf("0 - exit\n");
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
