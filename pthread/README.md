## Overview
Academic project featuring producer-consumer problem, semaphores and shared memory, implemented in C. The process contains four customers - one of them is a VIP, three producers and a deliver. Each of them is represented as a particular system process. There are also three FIFO queues: one for orders placed by customers with capacity of 10, one for products made by producers with capacity of 10, and one for tools with capacity of 2. Orders placed by VIP have higher priority than others, therefore product and order queues are priority queues.

## Usage
To start, compile main.cpp file. Remember to add -pthread flag.

```
g++ main.cpp -o main.out -pthread
```

Then call the output file. Two integers are required as parameters.

```
./main.out <order_interval> <priority_order_interval>
```

Mechanism of each process is described below.

1. Customer
A process responsible for placing order - putting an item to an order queue. Then, it randomizes a number from range [0.5*X, 1.5*X], where X is a parameter provided to the process while calling the program. For a VIP, the X is a number passed as a second program argument, and for others customers it is the number passed as a first argument. Finally, it sleeps for the randomized time and then places another order.

2. Producer
A process which takes an order - removes an item from order queue - then takes a tool from a tool queue (unless there is no free tool), works (sleep) for a time appropriate for taken tool and finally puts back the tool and produces a product - puts an item to products queue.

3. Deliver
A process with removes a product from product queue twice per second. Delivering the product to right customer itself is not a part of the project.

4. Tool
An item used to produce the product. There are two tools in a queue - A and B. If tool A is taken, producer sleeps for 1 second before creating the product. If tool B - the operating time is 2 seconds.
