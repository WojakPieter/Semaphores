#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/sem.h>
#include <time.h>
#include "queue.c"
#include <stdbool.h>

static int sem;
static struct sembuf buf[3];

int main(int argc, char *argv[]) {
    int init_waiting_time;
    int waiting_time;
    sscanf(argv[1], "%d", &init_waiting_time);
    struct OrderQueue *block = attach_memory_block_order();
    sem = semget(13, 3, IPC_CREAT | 0777);
    buf[0].sem_num = 0; //empty
    buf[1].sem_num = 1; //full
    buf[2].sem_num = 2; //mutex
    while (true) {
        struct Order order;
        order.order_time = time(NULL);
        buf[0].sem_op = -1;
        buf[1].sem_op = 1;
        buf[2].sem_op = -1;
        if (semop(sem, buf, 3) == -1) {
            printf("Some kind of error\n");
            exit(-1);
        }
        insert_to_order_queue(block, order);
        buf[2].sem_op = 1;
        if (semop(sem, &buf[2], 1) == -1) {
            printf("Some kind of error\n");
            exit(-1);
        }
        waiting_time = ((rand() % init_waiting_time) + 0.5 * init_waiting_time) * 1000000;
        usleep(waiting_time);
    }

    return 0;
}
