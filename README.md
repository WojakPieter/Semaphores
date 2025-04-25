## Overview
Academic project featuring producer-consumer problem, semaphores and shared memory, implemented in C. The process contains four customers, three producers and a deliver. Each of them is represented as a particular system process. There are also three FIFO queues: one for orders placed by customers with capacity of 10, one for products made by producers with capacity of 10, and one for tools with capacity of 2. Queues are implemented as shared memory blocks (from *sys* library in C). Synchronization is provided by semaphores (from *sys* library as well).

## Usage
To start, prepare three terminal windows. First, run script_init.sh script in one of terminals.

```
sh script_init.sh
```

In this terminal one can observe operating time for each product - how many seconds passed from placing an order to produce the product.

Then, in another terminal run script activating customers. Remember to pass a parameter - number of seconds for customer to wait before placing next order.

```
sh script_custromers.sh <integer>
```

In this terminal one can observe how many orders are right now in order queue.

Finally, in third terminal run script activating producers.

```
sh script_producers.sh
```

In this terminal one can observe the process of creating product - its id (order timestamp), taken tool, and quantity of products in a queue.

***Warning*** When finished, one should free memory blocks reserved for the program. To do so, call destroy_memory.o executable file

```
./destroy_memory.o
```

Mechanism of each process is described below.

1. Customer
A process responsible of placing order - putting an item to an order queue. Then, it randomizes a number from range [0.5*X, 1.5*X], where X is a parameter provided to the process while calling script_customers.sh. Finally, it sleeps for the randomized time and then places another order.

2. Producer
A process which takes an order - removes an item from order queue - then takes a tool from a tool queue (unless there is no free tool), works (sleep) for a time appropriate for taken tool and finally puts back the tool and produces a product - puts an item to products queue.

3. Deliver
A process with removes a product from product queue twice per second. Delivering the product to right customer itself is not a part of the project.

4. Tool
An item used to produce the product. There are two tools in a queue - A and B. If tool A is taken, producer sleeps for 1 second before creating the product. If tool B - the operating time is 2 seconds.