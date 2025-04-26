#include "dmalloc.hh"
#include <cstdio>
#include <cassert>
#include <cstring>
#include <iostream>
// heap_min and heap_max checking, simple case.

int main() {
    char* p = (char*) malloc(10);
    dmalloc_stats stat;
    get_statistics(&stat);
    assert((uintptr_t) p >= stat.heap_min);
    //std::cout << "max " << stat.heap_max << std::endl;
    assert((uintptr_t) p + 10 <= stat.heap_max);
}
