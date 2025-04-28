#include "order.h"
#include "product.h"
#include "tool.h"
#include "init_memory.c"
#include <stdbool.h>

#define SIZE 10

struct OrderQueue {
    int content_quantity;
    struct Order orders[SIZE];
};

struct ProductQueue {
    int content_quantity;
    struct Product products[SIZE];
};

struct ToolQueue {
    int content_quantity;
    struct Tool tools[2];
};

//zamowienia

void insert_to_order_queue(struct OrderQueue * queue, struct Order order) {
    printf("Order quantity: %d\n", queue->content_quantity);
    if (queue->content_quantity < SIZE) {
        queue->orders[queue->content_quantity] = order;
        queue->content_quantity++;
    }
}

struct Order get_from_order_queue(struct OrderQueue * queue) {
    if (queue->content_quantity > 0) {
        struct Order return_value = queue->orders[0];
        for (int i=0; i<queue->content_quantity; i++) {
            queue->orders[i] = queue->orders[i+1];
        }
        queue->content_quantity--;
        return return_value;
    }
}

bool is_order_full(struct OrderQueue * queue) {
    return (queue->content_quantity == SIZE);
}

//produkty

void insert_to_product_queue(struct ProductQueue * queue, struct Product product) {
    printf("Product quantity: %d\n", queue->content_quantity);
    if (queue->content_quantity < SIZE) {
        queue->products[queue->content_quantity] = product;
        queue->content_quantity++;
    }
}

struct Product get_from_product_queue(struct ProductQueue * queue) {
    if (queue->content_quantity > 0) {
        struct Product return_value = queue->products[0];
        for (int i=0; i<queue->content_quantity; i++) {
            queue->products[i] = queue->products[i+1];
        }
        queue->content_quantity--;
        return return_value;
    }
}

bool is_product_full(struct ProductQueue * queue) {
    return (queue->content_quantity == SIZE);
}

//narzedzia

void insert_to_tool_queue(struct ToolQueue * queue, struct Tool tool) {
    if (queue->content_quantity < 2) {
        queue->tools[queue->content_quantity] = tool;
        queue->content_quantity++;
    }
}

struct Tool get_from_tool_queue(struct ToolQueue * queue) {
    if (queue->content_quantity > 0) {
        struct Tool return_value = queue->tools[0];
        for (int i=0; i<queue->content_quantity; i++) {
            queue->tools[i] = queue->tools[i+1];
        }
        queue->content_quantity--;
        return return_value;
    }
}

bool is_tool_full(struct ToolQueue * queue) {
    return (queue->content_quantity == SIZE);
}
