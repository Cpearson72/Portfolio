#include "dmalloc.hh"
#include <cstdio>
// Trivial check: no allocations == zero statistics.

// we just copied test001.cc as an example!

int main() {
    //malicious setting of sz to a different value. how can we avoid to from destorying the program?

    void* p = malloc(10);
    size_t* size = (size_t*)((char*)p - sizeof(size_t)*2);

    *size = 134243;

    //should cause free errors
    free(p);

    print_statistics();
}

// Lines starting with "//!" define the expected output for this test.

//! alloc count: active          0   total          1   fail          0
//! alloc size:  active          0   total          10   fail          0
