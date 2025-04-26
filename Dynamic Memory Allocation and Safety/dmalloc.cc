#define DMALLOC_DISABLE 1
#include "dmalloc.hh"
#include <cassert>
#include <cstring>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <map>

//**GLOBAL DEC. OF VARIABLES TO BE USED IN PROGRAM**

//guard for overwrites
const size_t GUARD_VALUE = 0xDEADBEEF;
const size_t GUARD_SIZE = sizeof(size_t);

//size of allocated block metadata
size_t metadata_size = sizeof(size_t) * 2;

//sets that store pointers to allocated and freed blocks.
std::unordered_set<void*> allocated_blocks;
std::unordered_set<void*> freed_blocks;

//maps live ptr → (file, line) for printing information
static std::unordered_map<void*, std::pair<const char*,long>> allocation_info;

//bools used for min high and low buggy decleration behavior. forced to dec. in the funciton.
bool first_heap_min = true;
bool first_heap_max = true;


//define struct
//set global struct with all NULL values
dmalloc_stats* G_stats = (dmalloc_stats*)malloc(sizeof(dmalloc_stats));

std::map<void*,const size_t> sizes;

//**END OF GLOBALS */





/**
 * dmalloc(sz,file,line)
 *      malloc() wrapper. Dynamically allocate the requested amount `sz` of memory and 
 *      return a pointer to it 
 * 
 * @arg size_t sz : the amount of memory requested 
 * @arg const char *file : a string containing the filename from which dmalloc was called 
 * @arg long line : the line number from which dmalloc was called 
 * 
 * @return a pointer to the heap where the memory was reserved
 */
void* dmalloc(size_t sz, const char* file, long line) {
    (void) file, (void) line;   // avoid uninitialized variable warnings
   
    //Overflow protection
    if(sz >= ((size_t) - 1)){
        //update fail stats
        G_stats->fail_size += sz;
        G_stats->nfail += 1;
        return nullptr;
    }

    //Total size to be allocated with metadata, payload, and guard.
    const size_t total_size = sz + metadata_size + GUARD_SIZE;

    //Base call with failure check
    void* block = base_malloc(total_size);

    sizes.insert({block + metadata_size, sz});

    if(!block){
        G_stats->fail_size += sz;
        G_stats->nfail += 1;
        return nullptr;
    }

    //Lo and hi used to compare max and min heap
    uintptr_t lo = (uintptr_t)block + metadata_size;
    uintptr_t hi = (uintptr_t)block + total_size;
    
    //Comparisons
    if (first_heap_min || lo < G_stats->heap_min) {
        G_stats->heap_min = lo;
        first_heap_min    = false;
        }
    if (hi > G_stats->heap_max) {
        G_stats->heap_max = hi;
        }
    
    
    *(size_t*)block = sz;         // first slot
    *((size_t*)block + 1) = 1;    // second slot (“alive”)
   
    //Gather information from block to set Guard
    char*  payload_ptr = (char*)(block + metadata_size);
    size_t* right_guard = (size_t*)( payload_ptr + sz );
    *right_guard = GUARD_VALUE;
    

    /* STATS UPDATE CODE*/

    G_stats->nactive += 1;
    G_stats->ntotal += 1;
    G_stats->total_size += sz;
    G_stats->active_size += sz;

    /* STATS UPDATE CODE*/

    
    //Handle unordered sets that carry allocated ptrs that are alive
    allocated_blocks.insert((void*)((char*)block + metadata_size));



    //Remove from freed blocks if exists
    auto it2 = freed_blocks.find((void*)((char*)block + metadata_size));
    if(it2 != freed_blocks.end()){
        freed_blocks.erase(it2);
    }

    // track where this pointer came from
    allocation_info[block + metadata_size] = { file, line };

    return (void*)((char*)block + metadata_size);
}

/**
 * dfree(ptr, file, line)
 *      free() wrapper. Release the block of heap memory pointed to by `ptr`. This should 
 *      be a pointer that was previously allocated on the heap. If `ptr` is a nullptr do nothing. 
 * 
 * @arg void *ptr : a pointer to the heap 
 * @arg const char *file : a string containing the filename from which dfree was called 
 * @arg long line : the line number from which dfree was called 
 */


void not_in_heap(void* ptr, const char* file, long line){
    auto it = allocated_blocks.find(ptr);
    if(it == allocated_blocks.end()){
        if ((uintptr_t)ptr < G_stats->heap_min or (uintptr_t)ptr > G_stats->heap_max){
            std::cerr << "MEMORY BUG: " <<
               "invalid free of pointer " << ptr << ", not in heap" << std::endl;
            std::abort();
        }
    }
}


void double_free(void* ptr, const char* file, long line){
    //is ptr in freed blocks?
    auto it = freed_blocks.find(ptr);
    if(it != freed_blocks.end()){
        //std::cout << "ptr: " << ptr << std::endl;
        std::cerr << "MEMORY BUG: invalid free of pointer " << ptr << ", double free" << std::endl;
        std::abort();
    }
}


void free_payload_checker(void* ptr, const char* file, long line) {
    //Go through each alive payload, gather range, and test if ptr in range.
    for (auto curr_ptr : allocated_blocks) {

        //Gather allocated block information
        void* payload_ptr = curr_ptr;
        size_t* metadata_ptr = (size_t*)((char*)payload_ptr - metadata_size);
        //get size using ptr
        
        size_t sz;
        try { sz = sizes.at(curr_ptr);}
        catch (const std::out_of_range& e) { return;}
       

        size_t block_size = sz + metadata_size + sizeof(size_t);

        //Gather range
        void* payload_start = payload_ptr;
        void* payload_end = (void*)((char*)metadata_ptr + block_size);

        //If valid ptr, we are good.
        if (ptr == payload_start) {
            // Correct free
            return;
        }   

        //If not, and within range, we have advanced error.
        if ((ptr > payload_start) && (ptr < payload_end)) {
            // Invalid free inside block
            size_t offset = (uintptr_t)ptr - (uintptr_t)payload_start;
            size_t region_size = (uintptr_t)payload_end - (uintptr_t)payload_start - sizeof(size_t);
            
            
            std::cerr << "MEMORY BUG: " << file << ":" << line
                      << ": invalid free of pointer " << ptr << ", not allocated" << std::endl;

                      std::cerr << file << ":" << line - 1
                      << ": " << ptr << " is " << offset
                      << " bytes inside a " << region_size
                      << " byte region allocated here" << std::endl;
            
            std::abort();
        }
    }

    
}

void find_failures_and_edge_cases(void* ptr, const char* file, long line){
    

    //Failure edge cases
    not_in_heap(ptr, file, line);
    double_free(ptr, file, line);
    free_payload_checker(ptr, file, line);

    //Detect Guard overwrite
    size_t meta_size = *(size_t*)((char*)ptr - metadata_size);
    size_t sz;
        try { sz = sizes.at(ptr);}
        catch (const std::out_of_range& e) { return;}
       
    size_t* guard_when_free = (size_t*)( ptr + sz );

    //std::cout << curr_size << " " << meta_size << std::endl;
    
    if (*guard_when_free != GUARD_VALUE) {
        std::cerr << "MEMORY BUG: detected wild write during free of pointer "
                << ptr << std::endl;
        std::abort();
    }


}
void dfree(void* ptr, const char* file, long line) {
    (void) file, (void) line;   // avoid uninitialized variable warnings

     //NULLPTR edge case
     if(ptr == nullptr){
        return;
    }
    //Catch errors
    find_failures_and_edge_cases(ptr, file, line);

    //We have a success.
    size_t sz;
    try { sz = sizes.at(ptr);}
        catch (const std::out_of_range& e) { std::abort();}
    /* STATS UPDATE CODE*/
    G_stats->nactive -= 1;
    G_stats->active_size -= sz;
    //Need to figure out size of block

    /* STATS UPDATE CODE*/

    //Free total block and update sets
    void* real_block = (void*)((char*)ptr - metadata_size);
    allocated_blocks.erase(ptr);
    freed_blocks.insert(ptr);
    allocation_info.erase(ptr);
    sizes.erase(ptr);

    base_free(real_block);
}

/**
 * dcalloc(nmemb, sz, file, line)
 *      calloc() wrapper. Dynamically allocate enough memory to store an array of `nmemb` 
 *      number of elements with wach element being `sz` bytes. The memory should be initialized 
 *      to zero  
 * 
 * @arg size_t nmemb : the number of items that space is requested for
 * @arg size_t sz : the size in bytes of the items that space is requested for
 * @arg const char *file : a string containing the filename from which dcalloc was called 
 * @arg long line : the line number from which dcalloc was called 
 * 
 * @return a pointer to the heap where the memory was reserved
 */
void* dcalloc(size_t nmemb, size_t sz, const char* file, long line) {
    // Your code here (to fix test014).
    if (sz != 0 && nmemb > SIZE_MAX/sz) {
        G_stats->nfail++;
        G_stats->fail_size += (nmemb * sz);  // might overflow but that's ok for stats
        return nullptr;
    }

    void* ptr = dmalloc(nmemb * sz, file, line);
    if (ptr) {
        memset(ptr, 0, nmemb * sz);
    }
    return ptr;
}

/**
 * get_statistics(stats)
 *      fill a dmalloc_stats pointer with the current memory statistics  
 * 
 * @arg dmalloc_stats *stats : a pointer to the the dmalloc_stats struct we want to fill
 */
void get_statistics(dmalloc_stats* stats) {
    // Stub: set all statistics to enormous numbers
    memset(stats, 0, sizeof(dmalloc_stats));
    // Your code here.

    //use G_stats to update our stats struct
    if (G_stats) {
        //std::cout << "Active: " << G_stats->nactive << std::endl;
        stats->nactive = G_stats->nactive;
        stats->ntotal = G_stats->ntotal;
        stats->nfail = G_stats->nfail;
        stats->active_size = G_stats->active_size;
        stats->total_size = G_stats->total_size;
        stats->fail_size = G_stats->fail_size;
        stats->heap_min = G_stats->heap_min;
        stats->heap_max = G_stats->heap_max;
    }

}

/**
 * print_statistics()
 *      print the current memory statistics to stdout       
 */
void print_statistics() {
    dmalloc_stats stats;

    get_statistics(&stats);

    printf("alloc count: active %10llu   total %10llu   fail %10llu\n",
           stats.nactive, stats.ntotal, stats.nfail);
    printf("alloc size:  active %10llu   total %10llu   fail %10llu\n",
           stats.active_size, stats.total_size, stats.fail_size);
}

/**  
 * print_leak_report()
 *      Print a report of all currently-active allocated blocks of dynamic
 *      memory.
 */


void print_leak_report() {
    //Find all still active allocated ptrs
    for (void* ptr : allocated_blocks) {
        
        auto info = allocation_info.find(ptr);

        // recover size
        size_t* size = (size_t*)( (char*)ptr - metadata_size );
        size_t  sz  = *size;

        // print with file:line from when it was malloc'd
        std::cout
          << "LEAK CHECK: "
          << info->second.first 
          << ":" 
          << info->second.second
          << ": allocated object " 
          << ptr 
          << " with size " 
          << sz 
          << "\n";
    }
}




