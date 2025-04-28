#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <semaphore.h>
#include "queue.cpp"

#define SEMAPHORES_ORDER_BLOCK_ID (key_t)13
#define SEMAPHORES_PRODUCT_BLOCK_ID (key_t)14
#define SEMAPHORES_TOOL_BLOCK_ID (key_t)15

ToolQueue tool_queue;
OrderQueue order_queue;
ProductQueue product_queue;

struct customerArgs {
    bool vip;
    int sleeping_time;
};


sem_t full_order, empty_order;
sem_t full_product, empty_product;
sem_t full_tool, empty_tool;

sem_t mutex_order, mutex_product, mutex_tool;

void *customer(void* args_struct) {
    struct customerArgs *customer_args = (struct customerArgs *)args_struct;
    bool is_vip = customer_args->vip;
    while(true) {
        int sleeping_time = ((rand() % customer_args->sleeping_time) + 0.5 * customer_args->sleeping_time) * 1000000;
        struct Order order;
        order.vip = is_vip;
        order.order_time = time(NULL);
        sem_wait(&full_order);
        sem_wait(&mutex_order);
        insert_to_order_queue(&order_queue, order);
        sem_post(&mutex_order);
        sem_post(&empty_order);
        usleep(sleeping_time);
    }
    return 0;
}

void *deliver(void *arg) {
    while(true) {
        sem_wait(&empty_product);
        sem_wait(&mutex_product);
        struct Product product = get_from_product_queue(&product_queue);
        printf("Taken product. Order processing time: %ld, priority: %s\n", time(NULL) - product.calling_time, product.vip ? "high" : "low");
        sem_post(&mutex_product);
        sem_post(&full_product);
        usleep(500*1000);
    }
    return 0;
}

void *producer(void *arg) {
    while(true) {
        // take order
        sem_wait(&empty_order);
        sem_wait(&mutex_order);
        struct Order order = get_from_order_queue(&order_queue);
        sem_post(&mutex_order);
        sem_post(&full_order);

        // take tool
        sem_wait(&empty_tool);
        sem_wait(&mutex_tool);
        struct Tool tool = get_from_tool_queue(&tool_queue);
        sem_post(&mutex_tool);
            sem_post(&full_tool);

        sleep(tool.operating_time);
        struct Product product;
        product.calling_time = order.order_time;
        product.vip = order.vip;

        // return tool
        sem_wait(&full_tool);
        sem_wait(&mutex_tool);
        insert_to_tool_queue(&tool_queue, tool);
        sem_post(&mutex_tool);
        sem_post(&empty_tool);

        // put product
        sem_wait(&full_product);
        sem_wait(&mutex_product);
        insert_to_product_queue(&product_queue, product);
        sem_post(&mutex_product);
        sem_post(&empty_product);
        sleep(1);
    }
}

void init_semaphores() {
    sem_init(&full_order, 0, 10);
    sem_init(&empty_order, 0, 0);

    sem_init(&full_product, 0, 10);
    sem_init(&empty_product, 0, 0);

    sem_init(&full_tool, 0, 0);
    sem_init(&empty_tool, 0, 2);

    sem_init(&mutex_order, 0, 1);
    sem_init(&mutex_product, 0, 1);
    sem_init(&mutex_tool, 0, 1);
}

void on_exit(int s) {
    sem_destroy(&full_order);
    sem_destroy(&empty_order);

    sem_destroy(&full_product);
    sem_destroy(&empty_product);

    sem_destroy(&full_tool);
    sem_destroy(&empty_tool);
    exit(0);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Provide two parameters (integers)\n");
        exit(1);
    }
    struct Tool tool_a, tool_b;
    tool_a.id = 'A';
    tool_a.operating_time = 1;
    tool_b.id = 'B';
    tool_b.operating_time = 2;
    insert_to_tool_queue(&tool_queue, tool_a);
    insert_to_tool_queue(&tool_queue, tool_b);
    pthread_t producer1, producer2, producer3;
    pthread_t customer1, customer2, customer3, customer4;
    pthread_t deliver1;
    init_semaphores();
    int result = pthread_create(&deliver1, NULL, deliver, NULL);
    if (result != 0) {
        perror("Could not create thread.");
    }
    result = pthread_create(&producer1, NULL, producer, NULL);
    if (result != 0) {
        perror("Could not create thread.");
    }
    result = pthread_create(&producer2, NULL, producer, NULL);
    if (result != 0) {
        perror("Could not create thread.");
    }
    result = pthread_create(&producer3, NULL, producer, NULL);
    if (result != 0) {
        perror("Could not create thread.");
    }
    struct customerArgs nonvip_args, vip_args;
    sscanf(argv[1], "%d", &nonvip_args.sleeping_time);
    nonvip_args.vip = false;
    sscanf(argv[2], "%d", &vip_args.sleeping_time);
    vip_args.vip = true;
    result = pthread_create(&customer1, NULL, customer, (void *) &nonvip_args);
    if (result != 0) {
        perror("Could not create thread.");
    }
    result = pthread_create(&customer2, NULL, customer, (void *) &nonvip_args);
    if (result != 0) {
        perror("Could not create thread.");
    }
    result = pthread_create(&customer3, NULL, customer, (void *) &nonvip_args);
    if (result != 0) {
        perror("Could not create thread.");
    }
    result = pthread_create(&customer4, NULL, customer, (void *) &vip_args);
    if (result != 0) {
        perror("Could not create thread.");
    }
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = on_exit;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);
    pthread_join(producer1, NULL);
    pthread_join(producer2, NULL);
    pthread_join(producer3, NULL);

    pthread_join(customer1, NULL);
    pthread_join(customer2, NULL);
    pthread_join(customer3, NULL);
    pthread_join(customer4, NULL);

    pthread_join(deliver1, NULL);
    return 0;
}