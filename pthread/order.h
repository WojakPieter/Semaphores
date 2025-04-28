#include <sys/time.h>

struct Order {
    int number;
    time_t order_time;
    bool vip;
} order;