#include <sys/shm.h>
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>
#include "queue.c"

int main(int argc, char * argv[]) {
    // order
    sem_order = semget(SEMAPHORES_BOOKING_BLOCK_ID, 3, IPC_CREAT | 0777);
    buf_order[0].sem_num = 0; //empty
    buf_order[1].sem_num = 1; //full
    buf_order[2].sem_num = 2; //mutex
    arg.val = 10;
    semctl(sem_order, 0, SETVAL, arg);
    arg.val = 0;
    semctl(sem_order, 1, SETVAL, arg);
    arg.val = 1;
    semctl(sem_order, 2, SETVAL, arg);

    //product
    sem_product = semget(SEMAPHORES_PRODUCT_BLOCK_ID, 3, IPC_CREAT | 0777);
    buf_product[0].sem_num = 0; //empty
    buf_product[1].sem_num = 1; //full
    buf_product[2].sem_num = 2; //mutex
    arg.val = 10;
    semctl(sem_product, 0, SETVAL, arg);
    arg.val = 0;
    semctl(sem_product, 1, SETVAL, arg);
    arg.val = 1;
    semctl(sem_product, 2, SETVAL, arg);

    //tool
    sem_tool = semget(SEMAPHORES_TOOL_BLOCK_ID, 3, IPC_CREAT | 0777);
    buf_tool[0].sem_num = 0; //empty
    buf_tool[1].sem_num = 1; //full
    buf_tool[2].sem_num = 2; //mutex
    arg.val = 2;
    semctl(sem_tool, 0, SETVAL, arg);
    arg.val = 0;
    semctl(sem_tool, 1, SETVAL, arg);
    arg.val = 1;
    semctl(sem_tool, 2, SETVAL, arg);

    //tools init
    struct ToolQueue *tool_queue = attach_memory_block_tool();
    struct Tool tool_a;
    tool_a.id = 'A';
    tool_a.operating_time = 1;
    struct Tool tool_b;
    tool_b.id = 'B';
    tool_b.operating_time = 2;
    tool_queue->tools[0] = tool_a;
    tool_queue->tools[1] = tool_b;
    tool_queue->content_quantity = 2;

    struct ProductQueue *product_queue = attach_memory_block_product();
    product_queue->content_quantity = 0;

    struct OrderQueue *order_queue = attach_memory_block_order();
    order_queue->content_quantity = 0;
}