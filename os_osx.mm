#import <Cocoa/Cocoa.h>

NSString* applicationSupportDirectory();
NSString* findOrCreateDirectory(NSSearchPathDirectory searchPathDirectory, NSSearchPathDomainMask domainMask, NSString* appendComponent, NSError **errorOut);

#include "basecode.hpp"
#include "os.hpp"

// File reading & writing

OsFile open_game_file(const char *name, const char *mode){
#if ENGINE_DEVMODE
    char filename[MAX_PATH_LENGTH];
    sprintf(filename, "%s%s", RUN_TREE_PATH, name);
    FILE *fp = fopen(filename, mode);
#else
    NSString *path = [[[[NSBundle mainBundle] resourcePath] stringByAppendingString:@"/"] stringByAppendingString:[NSString stringWithUTF8String:name]];
    FILE *fp = fopen([path UTF8String], mode);
#endif
    return fp;
}
OsFile open_user_file(const char *name, const char *mode){
    NSString *path = [[applicationSupportDirectory() stringByAppendingString:@"/"] stringByAppendingString:[NSString stringWithUTF8String:name]];
    FILE *fp = fopen([path UTF8String], mode);
    return fp;
}
OsFile open_file(const char *name, const char *mode){
    FILE *fp = fopen(name, mode);
    return fp;
}

void get_game_file_path(const char *name, char *dst){
#if ENGINE_DEVMODE
    sprintf(dst, "%s%s", RUN_TREE_PATH, name);
#else
    const char *path = [[[NSBundle mainBundle] resourcePath] UTF8String];
    sprintf(dst, "%s/%s", path, name);
#endif
}
void get_user_file_path(const char *name, char *dst){
    const char *path = [applicationSupportDirectory() UTF8String];
    sprintf(dst, "%s/%s", path, name);
}

void read_file(void *dest, unsigned int size, unsigned int amount, OsFile fp){
    fread(dest, size, amount, fp);
}
void write_file(void *dest, unsigned int size, unsigned int amount, OsFile fp){
    fwrite(dest, size, amount, fp);
}
void close_file(OsFile fp){
    fclose(fp);
}

// Directories
// https://www.cocoawithlove.com/2010/05/finding-or-creating-application-support.html
/*

void createUserDirectory(const char *name){
    NSString *path = [[applicationSupportDirectory() stringByAppendingString:@"/"] stringByAppendingString:[NSString stringWithUTF8String:name]];
    NSError *error;
    [[NSFileManager defaultManager]
        createDirectoryAtPath: path
        withIntermediateDirectories:YES
        attributes:nil
     error:&error];
    if (error)
    {
        NSLog(@"Unable to create directory:\n%@", error);
    }
}*/

NSString* findOrCreateDirectory(NSSearchPathDirectory searchPathDirectory, NSSearchPathDomainMask domainMask, NSString* appendComponent, NSError **errorOut){
    // Search for the path
    NSArray* paths = NSSearchPathForDirectoriesInDomains(
                                                         searchPathDirectory,
                                                         domainMask,
                                                         YES);
    if ([paths count] == 0)
    {
        // *** creation and return of error object omitted for space
        return nil;
    }
    
    // Normally only need the first path
    NSString *resolvedPath = [paths objectAtIndex:0];
    
    if (appendComponent)
    {
        resolvedPath = [resolvedPath
                        stringByAppendingPathComponent:appendComponent];
    }
    
    // Create the path if it doesn't exist
    NSError *error;
    BOOL success = [[NSFileManager defaultManager]
                    createDirectoryAtPath:resolvedPath
                    withIntermediateDirectories:YES
                    attributes:nil
                    error:&error];
    if (!success)
    {
        if (errorOut)
        {
            *errorOut = error;
        }
        return nil;
    }
    
    // If we've made it this far, we have a success
    if (errorOut)
    {
        *errorOut = nil;
    }
    return resolvedPath;
}

NSString* applicationSupportDirectory(){
    NSString *executableName =
    [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleExecutable"];
    NSError *error;
    NSString *result = findOrCreateDirectory(NSApplicationSupportDirectory, NSUserDomainMask, executableName, &error);
    if (error)
    {
        NSLog(@"Unable to find or create application support directory:\n%@", error);
    }
    NSLog(@"App sup: %@", result);
    return result;
}


OsTime file_modification_date(const char *cpath){
    NSString *path = [NSString stringWithUTF8String:cpath];
    
    NSError *error = nil;
    NSDictionary *attrs = [[NSFileManager defaultManager] attributesOfItemAtPath:path error:&error];
    if(attrs && !error){
        return [[attrs fileModificationDate] timeIntervalSince1970];
    }
    return 0;
}

//
// Threads job system
//
// https://github.com/itfrombit/osx_handmade_minimal/blob/master/code/osx_handmade_thread.cpp
#include <pthread.h>
void* queue_thread_proc(void *data){
    //platform_work_queue* Queue = (platform_work_queue*)data;
    ThreadStartUp* thread = (ThreadStartUp*)data;
    WorkQueue* queue = thread->queue;
    
    for(;;){
        if(do_next_work_queue_entry(queue)){
            dispatch_semaphore_wait(queue->semaphore_handle, DISPATCH_TIME_FOREVER);
        }
    }
    
    return(0);
}



void make_queue(WorkQueue* queue, uint32 thread_count, ThreadStartUp* startups){
    queue->completion_goal = 0;
    queue->completion_count = 0;
    
    queue->next_entry_to_write = 0;
    queue->next_entry_to_read = 0;
    
    queue->semaphore_handle = dispatch_semaphore_create(0);
    
    for (uint32 ThreadIndex = 0;
         ThreadIndex < thread_count;
         ++ThreadIndex){
        ThreadStartUp* startup = startups + ThreadIndex;
        startup->queue = queue;
        
        pthread_t        ThreadId;
        int r = pthread_create(&ThreadId, NULL, queue_thread_proc, startup);
        if (r != 0){
            printf("Error creating thread %d\n", ThreadIndex);
        }
    }
}


void add_entry(WorkQueue* queue, WorkQueueCallback* callback, void* data){
    // TODO(casey): Switch to InterlockedCompareExchange eventually
    // so that any thread can add?
    uint32 new_next_entry_to_write = (queue->next_entry_to_write + 1) % ArrayCount(queue->entries);
    assert(new_next_entry_to_write != queue->next_entry_to_read);
    WorkQueueEntry *entry = queue->entries + queue->next_entry_to_write;
    entry->callback = callback;
    entry->data = data;
    ++queue->completion_goal;
    OSMemoryBarrier();
    // Not needed: _mm_sfence();
    queue->next_entry_to_write = new_next_entry_to_write;
    dispatch_semaphore_signal(queue->semaphore_handle);
    
#if 0
    int r = dispatch_semaphore_signal(queue->semaphore_handle);
    if(r > 0){
        printf("  dispatch_semaphore_signal: A thread was woken\n");
    }else{
        printf("  dispatch_semaphore_signal: No thread was woken\n");
    }
#endif
}

bool do_next_work_queue_entry(WorkQueue* queue){
    bool WeShouldSleep = false;
    
    uint32 OriginalNextEntryToRead = queue->next_entry_to_read;
    uint32 NewNextEntryToRead = (OriginalNextEntryToRead + 1) % ArrayCount(queue->entries);
    
    if(OriginalNextEntryToRead != queue->next_entry_to_write){
        // NOTE(jeff): OSAtomicCompareAndSwapXXX functions return 1 if the swap took place, 0 otherwise!
        uint32 SwapOccurred = OSAtomicCompareAndSwapIntBarrier(OriginalNextEntryToRead,
                                                               NewNextEntryToRead,
                                                               (int volatile*)&queue->next_entry_to_read);
        
        if(SwapOccurred){
            WorkQueueEntry Entry = queue->entries[OriginalNextEntryToRead];
            Entry.callback(queue, Entry.data);
            //InterlockedIncrement((int volatile *)&Queue->CompletionCount);
            OSAtomicIncrement32Barrier((int volatile *)&queue->completion_count);
        }else{
        }
    }else{
        WeShouldSleep = true;
    }
    
    return(WeShouldSleep);
}

void complete_all_work(WorkQueue *queue){
    while (queue->completion_goal != queue->completion_count){
        do_next_work_queue_entry(queue);
    }
    
    queue->completion_goal = 0;
    queue->completion_count = 0;
}
