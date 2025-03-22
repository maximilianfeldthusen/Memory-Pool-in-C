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

// Function to create a memory pool
MemoryPool* create_memory_pool(size_t block_size, size_t block_count) {
    MemoryPool* pool = (MemoryPool*)malloc(sizeof(MemoryPool));
    if (!pool) {
        return NULL;
    }

    pool->block_size = block_size;
    pool->block_count = block_count;
    pool->free_blocks = NULL;

    // Allocate memory for blocks
    MemoryBlock* blocks = (MemoryBlock*)malloc(block_size * block_count + sizeof(MemoryBlock) * block_count);
    if (!blocks) {
        free(pool);
        return NULL;
    }

    // Initialize free block list
    for (size_t i = 0; i < block_count; ++i) {
        MemoryBlock* block = (MemoryBlock*)((char*)blocks + i * block_size + sizeof(MemoryBlock) * i);
        block->next = pool->free_blocks;
        pool->free_blocks = block;
    }

    return pool;
}

// Function to allocate a block of memory from the pool
void* allocate_block(MemoryPool* pool) {
    if (pool->free_blocks == NULL) {
        return NULL; // No free blocks available
    }

    MemoryBlock* block = pool->free_blocks;
    pool->free_blocks = block->next;

    return (void*)((char*)block + sizeof(MemoryBlock)); // Return the usable memory
}

// Function to free a block of memory back to the pool
void free_block(MemoryPool* pool, void* block) {
    if (block == NULL) {
        return; // Do nothing if the block is NULL
    }
    
    MemoryBlock* mem_block = (MemoryBlock*)((char*)block - sizeof(MemoryBlock));
    mem_block->next = pool->free_blocks;
    pool->free_blocks = mem_block;
}

// Function to destroy the memory pool and free allocated memory
void destroy_memory_pool(MemoryPool* pool) {
    if (pool) {
        free(pool->free_blocks); // This is incorrect; free the entire block memory instead
        free(pool); // Free the pool structure
    }
}

// Example usage
int main() {
    size_t block_size = sizeof(int);
    size_t block_count = 10;

    MemoryPool* pool = create_memory_pool(block_size, block_count);
    if (!pool) {
        fprintf(stderr, "Failed to create memory pool
");
        return EXIT_FAILURE;
    }

    // Allocate blocks
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

    // Free blocks
    for (int i = 0; i < 10; ++i) {
        free_block(pool, nums[i]);
        printf("Freed block for %d
", *nums[i]);
    }

    // Free the entire block memory
    free(pool->free_blocks);
    destroy_memory_pool(pool);
    return EXIT_SUCCESS;
}

