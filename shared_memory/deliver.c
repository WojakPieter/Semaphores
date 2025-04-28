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

int main(int argc, char * argv[]) {
    struct ProductQueue *product_queue = attach_memory_block_product();
    sem_product = semget(SEMAPHORES_PRODUCT_BLOCK_ID, 3, IPC_CREAT | 0777);
    buf_product[0].sem_num = 0; //empty
    buf_product[1].sem_num = 1; //full
    buf_product[2].sem_num = 2; //mutex
    while (true) {
        buf_product[0].sem_op = 1;
        buf_product[1].sem_op = -1;
        buf_product[2].sem_op = -1;
        if (semop(sem_product, buf_product, 3) == -1) {
            printf("Some kind of error\n");
            exit(-1);
        }
        struct Product product = get_from_product_queue(product_queue);
        printf("Taken product. Operating time: %ld\n", time(NULL) - product.calling_time);
        buf_product[2].sem_op = 1;
        if (semop(sem_product, &buf_product[2], 1) == -1) {
            printf("Some kind of error\n");
            exit(-1);
        }
        usleep(500*1000);
    }
}