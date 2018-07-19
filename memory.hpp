#ifndef __engine__memory__
#define __engine__memory__

#define KB(n) n << 10
#define MB(n) n << 20
#define GB(n) n << 30

struct Allocator{
    void *(*allocate)(Allocator *allocator, u32 size);
    void (*deallocate)(Allocator *allocator, void *p);
};

struct NormalAllocator{
    Allocator allocator;
    NormalAllocator();
};

struct MemoryPool{
    Allocator allocator;
    
    uint64 size; // Reserved size
    uint64 occupied;
    uint64 high_water_mark;
    uint8 *memory;
    
    MemoryPool();
    MemoryPool(uint64 s);
    MemoryPool(uint64 s, uint8 *p);
};

extern Allocator default_allocator;
extern MemoryPool temporary_storage;

#define start_temp_alloc() u64 __xdeg__starting_temporary_storage_mark = temporary_storage.occupied
#define end_temp_alloc() temporary_storage.occupied = __xdeg__starting_temporary_storage_mark
#define temp_alloc(size) temporary_storage.allocator.allocate(&temporary_storage.allocator, size)

void reset_memory_pool(MemoryPool &ma);
void free_memory(MemoryPool ma);

void init_basecode_memory();
void cleanup_basecode_memory();
MemoryPool get_memory_pool(u64 size);

#endif /* defined(__engine__memory__) */
