#pragma once

#include "ordered_array.h"

#include <stdint.h>

#define KHEAP_START         0xC0000000
#define KHEAP_INITIAL_SIZE  0x100000
#define HEAP_INDEX_SIZE   0x20000
#define HEAP_MAGIC        0x123890AB
#define HEAP_MIN_SIZE     0x70000

typedef struct
{
   ordered_array_t index;
   uint32_t start_address; // The start of our allocated space.
   uint32_t end_address;   // The end of our allocated space. May be expanded up to max_address.
   uint32_t max_address;   // The maximum address the heap can be expanded to.
   uint8_t supervisor;     // Should extra pages requested by us be mapped as supervisor-only?
   uint8_t readonly;       // Should extra pages requested by us be mapped as read-only?
} heap_t;

/**
  Create a new heap.
**/
heap_t *create_heap(uint32_t start, uint32_t end, uint32_t max, uint8_t supervisor, uint8_t readonly);
/**
  Allocates a contiguous region of memory 'size' in size. If page_align==1, it creates that block starting
  on a page boundary.
**/
void *alloc(uint32_t size, uint8_t page_align, heap_t *heap);
/**
  Releases a block allocated with 'alloc'.
**/
void free(void *p, heap_t *heap);

uint32_t kmalloc_a(uint32_t sz);  // page aligned.
uint32_t kmalloc_p(uint32_t sz, uint32_t *phys); // returns a physical address.
uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys); // page aligned and returns a physical address.
uint32_t kmalloc(uint32_t sz); // vanilla (normal).

void kfree(void* ptr);