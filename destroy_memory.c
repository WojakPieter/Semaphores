#include <sys/shm.h>
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/sem.h>
#include <stdlib.h>
#include "init_memory.c"

int main(int argc, char *argv[]) {
    int shared_block_id = shmget(SHARED_BLOCK_BOOKING_ID, 4096, IPC_CREAT | 0777);
    shmctl(shared_block_id, IPC_RMID, NULL);
    shared_block_id = shmget(SHARED_BLOCK_PRODUCT_ID, 4096, IPC_CREAT | 0777);
    shmctl(shared_block_id, IPC_RMID, NULL);
    shared_block_id = shmget(SHARED_BLOCK_TOOL_ID, 4096, IPC_CREAT | 0777);
    shmctl(shared_block_id, IPC_RMID, NULL);

    int sem_block = semget(SEMAPHORES_BOOKING_BLOCK_ID, 3, IPC_CREAT | 0777);
    semctl(sem_block, 0, IPC_RMID);
    sem_block = semget(SEMAPHORES_PRODUCT_BLOCK_ID, 3, IPC_CREAT | 0777);
    semctl(sem_block, 0, IPC_RMID);
    sem_block = semget(SEMAPHORES_TOOL_BLOCK_ID, 3, IPC_CREAT | 0777);
    semctl(sem_block, 0, IPC_RMID);
    return 0;
}
