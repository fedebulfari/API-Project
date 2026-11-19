# Degree Project - Algorithms and Principles of Computer Science (API)

## Description
A time-stepped event-driven simulation for managing a production, warehousing, and logistics system. The system processes the procurement of perishable ingredients, fulfills orders based on predefined recipes, and manages periodic shipments via courier, respecting capacity constraints and deadlines.

## Architecture and Data Structures
The project utilizes advanced data structures written entirely in pure C to maximize temporal and spatial performance:

*   **Hash Table:** Implemented for the *Recipe Book* and the *Warehouse* (capacity H = 9999999). The hashing function leverages the multiplication method with the golden ratio constant. Collision resolution is handled via chaining with dynamically allocated arrays.
*   **Min-Heap (`s_heap`):** Used to manage ingredient expiration dates. It ensures that batches with the earliest expiration dates are consumed first, allowing extractions and insertions in $O(\log S)$ time.
*   **Max-Heap (`heap`):** The structure governing the courier's loading logic. Orders are extracted giving absolute priority to decreasing weight and, in the case of equal weight, to the lowest system time (FIFO), with logarithmic complexity relative to the available orders.
*   **Queues (Linked Lists):**
    *   *Singly Linked List (`queue_o`)*: Sequential and ordered management of confirmed orders ready for shipment.
    *   *Doubly Linked List (`queue`)*: Management of the waiting queue for orders lacking sufficient ingredients. The doubly linked structure ensures an $O(1)$ node detachment when an order is unblocked following a warehouse restock.

## Operations and Commands
The program waits on standard input for the initial courier setup parameters (periodicity and maximum capacity) and subsequently for a continuous stream of time events:

*   **Add Recipe (prefix `a`):** Registers a new recipe by specifying its name, required ingredients, and relative weight.
*   **Remove Recipe (prefix `r m`):** Logically removes (via a *tombstone* flag) a recipe from the system. The operation fails if there are pending orders for that recipe.
*   **Restock (prefix `r f`):** Inserts batches of raw materials into the warehouse, defining their quantity and expiration date. This operation triggers a sequential check on the waiting queue to fulfill any previously blocked orders.
*   **Order (prefix `o`):** Receipt of a production request. The system immediately deducts the necessary quantities from the batches with valid expiration dates; if the warehouse does not have sufficient resources, the order is parked in the waiting queue.

## Compilation
The source code is optimized for POSIX/GNU environments. For the production build, using the `gcc` compiler with the highest level of standardization and the memory sanitization flags included in the source is recommended.

```bash
gcc -Wall -Werror -std=gnu11 -O2 -lm -g3 -fsanitize=address "Progetto di Laurea API.c" -o progetto_api
```
# Memory Leak Analysis
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./progetto_api

# Performance Profiling (Callgrind)
valgrind --tool=callgrind ./progetto_api

## AI usage in development

0%
