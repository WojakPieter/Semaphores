#include <sys/shm.h>
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>

#define SHARED_BLOCK_BOOKING_ID (key_t)10
#define SHARED_BLOCK_PRODUCT_ID (key_t)11
#define SHARED_BLOCK_TOOL_ID (key_t)12
#define SHARED_BLOCK_QUEUE_SIZE (size_t)4096

#define SEMAPHORES_BOOKING_BLOCK_ID (key_t)13
#define SEMAPHORES_PRODUCT_BLOCK_ID (key_t)14
#define SEMAPHORES_TOOL_BLOCK_ID (key_t)15


static int sem_order;
static struct sembuf buf_order[3];
static int sem_tool;
static struct sembuf buf_tool[3];
static int sem_product;
static struct sembuf buf_product[3];
union semun {
    int val;               /* used for SETVAL only */
    struct semid_ds *buf;  /* used for IPC_STAT and IPC_SET */
    short *array;         /* used for GETALL and SETALL */
} arg;

static int get_shared_block_order() {
    return shmget(SHARED_BLOCK_BOOKING_ID, SHARED_BLOCK_QUEUE_SIZE, IPC_CREAT | 0777);
}

struct OrderQueue * attach_memory_block_order() {
    int shared_block_id = get_shared_block_order();
    struct OrderQueue *result;

    if (shared_block_id == -1) {
        printf("Shared block id = -1\n");
        exit(-1);
    }
    result = shmat(shared_block_id, NULL, 0);
    return result;
}

static int get_shared_block_product() {
    return shmget(SHARED_BLOCK_PRODUCT_ID, SHARED_BLOCK_QUEUE_SIZE, IPC_CREAT | 0777);
}

struct ProductQueue * attach_memory_block_product() {
    int shared_block_id = get_shared_block_product();
    struct ProductQueue *result;

    if (shared_block_id == -1) {
        printf("Shared block id = -1\n");
        exit(-1);
    }
    result = shmat(shared_block_id, NULL, 0);
    return result;
}

static int get_shared_block_tool() {
    return shmget(SHARED_BLOCK_TOOL_ID, SHARED_BLOCK_QUEUE_SIZE, IPC_CREAT | 0777);
}

struct ToolQueue * attach_memory_block_tool() {
    int shared_block_id = get_shared_block_tool();
    struct ToolQueue *result;

    if (shared_block_id == -1) {
        printf("Shared block id = -1\n");
        exit(-1);
    }
    result = shmat(shared_block_id, NULL, 0);
    return result;
}
