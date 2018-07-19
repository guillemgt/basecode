#include <assert.h>

#include "basecode.hpp"
#include "memory.hpp"

MemoryPool total_heap_memory;
u8 *total_heap_memory_next_section;

// Normal allocator
void * normal_allocate(Allocator *allocator, u32 size){
    if(size > 0){
        //printf("alloc\n"); // @Todo: Don't do so many allocations!!!!!!!!!!!!!!!!
        void *p = malloc(size);
        //printf("Allocating %p [size = %u]\n", p, size);
        return p;
    }else
        return nullptr;
}
void normal_deallocate(Allocator *allocator, void *p){
    //printf("Freeing %p\n", p);
    free(p);
}
NormalAllocator::NormalAllocator(){
    allocator.allocate = &normal_allocate;
    allocator.deallocate = &normal_deallocate;
}

Allocator default_allocator = {
    &normal_allocate,
    &normal_deallocate
};

MemoryPool temporary_storage;

// Temp allocator
void * temp_allocate(Allocator *alloc, u32 s){
    MemoryPool *allocator = (MemoryPool *)alloc;
    u8 *t = allocator->memory+allocator->occupied;
    allocator->occupied += s;
    assert(allocator->occupied <= allocator->size);
    if(allocator->occupied > allocator->high_water_mark)
        allocator->high_water_mark = allocator->occupied;
    return t;
}
void temp_deallocate(Allocator *alloc, void *p){
    
}
void reset_memory_pool(MemoryPool &ma){
    ma.occupied = 0;
}
MemoryPool::MemoryPool(uint64 s){
    occupied = 0;
    high_water_mark = 0;
    size = s;
    memory = (uint8 *)malloc(s*sizeof(uint8));
    
    allocator.allocate = &temp_allocate;
    allocator.deallocate = &temp_deallocate;
}
MemoryPool::MemoryPool(uint64 s, uint8 *p){
    occupied = 0;
    high_water_mark = 0;
    size = s;
    memory = p;
    
    allocator.allocate = &temp_allocate;
    allocator.deallocate = &temp_deallocate;
}

MemoryPool::MemoryPool(){
    occupied = 0;
    high_water_mark = 0;
    size = 0;
    memory = nullptr;
    
    allocator.allocate = nullptr;
    allocator.deallocate = nullptr;
}

void free_memory(MemoryPool ma){
    free(ma.memory);
}



void init_basecode_memory(){
    total_heap_memory = MemoryPool(MB(14L));
    total_heap_memory_next_section = (u8 *) total_heap_memory.memory;
    temporary_storage = get_memory_pool(KB(13990L));
}
MemoryPool get_memory_pool(u64 size){
    MemoryPool ma = MemoryPool(size, total_heap_memory_next_section);
    total_heap_memory_next_section += size;
    return ma;
}
void cleanup_basecode_memory(){
    free_memory(total_heap_memory);
}
