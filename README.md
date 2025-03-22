
## Documentation

### Memory-Pool-in-C

The code implements a simple memory pool in C, which is a technique for managing memory allocations and deallocations efficiently. This is particularly useful in applications with frequent allocations and deallocations of memory blocks of the same size. Let’s break down the code step by step.

### 1. **Includes and Type Definitions**
```c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct MemoryBlock {
    struct MemoryBlock* next;
} MemoryBlock;

typedef struct MemoryPool {
    size_t block_size;
    size_t block_count;
    MemoryBlock* free_blocks;
} MemoryPool;
```
- The code includes standard libraries for input/output (`stdio.h`), memory management (`stdlib.h`), Boolean type (`stdbool.h`), and string manipulation (`string.h`).
- It defines a structure `MemoryBlock`, which contains a pointer to the next block in the free list.
- The `MemoryPool` structure keeps track of the size of each memory block, the total number of blocks, and a pointer to the list of free blocks.

### 2. **Creating a Memory Pool**
```c
MemoryPool* create_memory_pool(size_t block_size, size_t block_count) {
    MemoryPool* pool = (MemoryPool*)malloc(sizeof(MemoryPool));
    if (!pool) {
        return NULL;
    }

    pool->block_size = block_size;
    pool->block_count = block_count;
    pool->free_blocks = NULL;

    MemoryBlock* blocks = (MemoryBlock*)malloc(block_size * block_count + sizeof(MemoryBlock) * block_count);
    if (!blocks) {
        free(pool);
        return NULL;
    }

    for (size_t i = 0; i < block_count; ++i) {
        MemoryBlock* block = (MemoryBlock*)((char*)blocks + i * block_size + sizeof(MemoryBlock) * i);
        block->next = pool->free_blocks;
        pool->free_blocks = block;
    }

    return pool;
}
```
- This function allocates a memory pool with a specified block size and count.
- It allocates enough memory for all the blocks and their metadata.
- It initializes the free blocks list by linking all allocated blocks together.

### 3. **Allocating a Block of Memory**
```c
void* allocate_block(MemoryPool* pool) {
    if (pool->free_blocks == NULL) {
        return NULL; // No free blocks available
    }

    MemoryBlock* block = pool->free_blocks;
    pool->free_blocks = block->next;

    return (void*)((char*)block + sizeof(MemoryBlock)); // Return usable memory
}
```
- This function retrieves a block from the list of free blocks. 
- If there are no free blocks, it returns `NULL`.
- It returns a pointer to the usable memory by offsetting the returned block pointer by the size of `MemoryBlock`.

### 4. **Freeing a Block of Memory**
```c
void free_block(MemoryPool* pool, void* block) {
    if (block == NULL) {
        return; // Do nothing if the block is NULL
    }
    
    MemoryBlock* mem_block = (MemoryBlock*)((char*)block - sizeof(MemoryBlock));
    mem_block->next = pool->free_blocks;
    pool->free_blocks = mem_block;
}
```
- This function adds a previously allocated block back to the free list.
- It calculates the pointer to the `MemoryBlock` structure using the pointer to the usable memory.

### 5. **Destroying the Memory Pool**
```c
void destroy_memory_pool(MemoryPool* pool) {
    if (pool) {
        free(pool->free_blocks); // This is incorrect; free the entire block memory instead
        free(pool); // Free the pool structure
    }
}
```
- This function is meant to clean up all allocated memory when the pool is no longer needed.
- However, it incorrectly attempts to free only the `free_blocks`, which would lead to a memory leak since it does not free the entire block of memory initially allocated for the blocks.

### 6. **Example Usage**
```c
int main() {
    size_t block_size = sizeof(int);
    size_t block_count = 10;

    MemoryPool* pool = create_memory_pool(block_size, block_count);
    if (!pool) {
        fprintf(stderr, "Failed to create memory pool
");
        return EXIT_FAILURE;
    }

    int* nums[10];
    for (int i = 0; i < 10; ++i) {
        nums[i] = (int*)allocate_block(pool);
        if (nums[i]) {
            *nums[i] = i; // Initialize the allocated memory
            printf("Allocated block for %d
", *nums[i]);
        } else {
            printf("Failed to allocate block
");
        }
    }

    for (int i = 0; i < 10; ++i) {
        free_block(pool, nums[i]);
        printf("Freed block for %d
", *nums[i]);
    }

    destroy_memory_pool(pool);
    return EXIT_SUCCESS;
}
```
- The `main` function creates a memory pool for integers, allocates 10 blocks, initializes them, and then frees them.
- It attempts to destroy the memory pool at the end, but it has an issue in the `destroy_memory_pool` function as mentioned earlier.

### Summary
Overall, this code provides a simple implementation of a memory pool in C, which can efficiently manage memory allocations for fixed-size blocks. Although the core functionality is present, the `destroy_memory_pool` function has a critical bug that needs to be addressed to avoid memory leaks.

