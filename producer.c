#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>
#include "queue.c"
#include <stdbool.h>

int main(int argc, char *argv[]) {
    struct OrderQueue *order_queue = attach_memory_block_order();
    struct ToolQueue *tool_queue = attach_memory_block_tool();
    struct ProductQueue *product_queue = attach_memory_block_product();
    sem_order = semget(SEMAPHORES_BOOKING_BLOCK_ID, 3, IPC_CREAT | 0777);
    buf_order[0].sem_num = 0; //empty
    buf_order[1].sem_num = 1; //full
    buf_order[2].sem_num = 2; //mutex
    sem_product = semget(SEMAPHORES_PRODUCT_BLOCK_ID, 3, IPC_CREAT | 0777);
    buf_product[0].sem_num = 0; //empty
    buf_product[1].sem_num = 1; //full
    buf_product[2].sem_num = 2; //mutex
    sem_tool = semget(SEMAPHORES_TOOL_BLOCK_ID, 3, IPC_CREAT | 0777);
    buf_tool[0].sem_num = 0; //empty
    buf_tool[1].sem_num = 1; //full
    buf_tool[2].sem_num = 2; //mutex
    while (true) {
        buf_order[1].sem_op = -1;
        buf_order[0].sem_op = 1;
        buf_order[2].sem_op = -1;
        if (semop(sem_order, buf_order, 3) == -1) {
            printf("Some kind of error\n");
            exit(-1);
        }
        struct Order order = get_from_order_queue(order_queue);
        printf("============================\n");
        printf("Desc: %ld\n",order.order_time);
        buf_order[2].sem_op = 1;
        if (semop(sem_order, &buf_order[2], 1) == -1) {
            printf("Some kind of error\n");
            exit(-1);
        }
        buf_tool[0].sem_op = -1;
        buf_tool[1].sem_op = 1;
        buf_tool[2].sem_op = -1;
        if (semop(sem_tool, buf_tool, 3) == -1) {
            printf("Some kind of error\n");
            exit(-1);
        }
        struct Tool tool = get_from_tool_queue(tool_queue);
        printf("Taken tool: %c\n", tool.id);
        buf_tool[2].sem_op = 1;
        if (semop(sem_tool, &buf_tool[2], 1) == -1) {
            printf("Some kind of error\n");
            exit(-1);
        }
        sleep(tool.operating_time);
        buf_tool[1].sem_op = -1;
        buf_tool[0].sem_op = 1;
        buf_tool[2].sem_op = -1;
        if (semop(sem_tool, buf_tool, 3) == -1) {
            printf("Some kind of error\n");
            exit(-1);
        }
        insert_to_tool_queue(tool_queue, tool);
        buf_tool[2].sem_op = 1;
        if (semop(sem_tool, &buf_tool[2], 1) == -1) {
            printf("Some kind of error\n");
            exit(-1);
        }
        printf("Tool returned\n");
        struct Product product;
        product.calling_time = order.order_time;
        buf_product[0].sem_op = -1;
        buf_product[1].sem_op = 1;
        buf_product[2].sem_op = -1;
        if (semop(sem_product, buf_product, 3) == -1) {
            printf("Some kind of error\n");
            exit(-1);
        }
        insert_to_product_queue(product_queue, product);
        buf_product[2].sem_op = 1;
        if (semop(sem_product, &buf_product[2], 1) == -1) {
            printf("Some kind of error\n");
            exit(-1);
        }
        printf("\n\n");
        sleep(1);
    }
    return 0;
}