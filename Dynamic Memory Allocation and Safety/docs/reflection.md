# What changed between your design document and the final implementation.

The first large change was my expectation of how many errors I must check. I had not considered wild writes, invalid frees within allocated blocks, and the introduction of advanced reporting. My designed doc only specified these two functions to account for allocation based errors:

### Design Doc Functions

bool invalid_free(*mem_ptr, size):

- will traverse a block of previously allocated memory and examine the meta data. This meta data, I believe, will store the life time and current statistic on this allocated block. From there I can examine if this block has already been allocated or not.

bool boundary_write_error(*mem_ptr, size):

- first determine boundaries of said memory block and then use the size of the read/write command to determine if we would go outside these boundaries. I am unsure if the location of the read/write will have a major effect, but if it does, I can pass another argument that returns a pointer to where the memory will exactly be read/written to.

    
While I had a good conceptual idea of where I needed to in terms of error detection with the use of metadata, I was clueless as to the number of errors I needed to detect. I ended up addming many addition functions, including.

### Final Implementation Functions

void out_of_bounds_free(void* ptr, const char* file, long line)

- Detects frees within the heap that are not allocated payload pointers. This mean that the memory has not been allocated in that position due to a prior free or a poor ptr being passed that lives within an allocated block.

void not_in_heap(void* ptr, const char* file, long line)

- Similar to out of bounds, but checks if ptr is outside of the heap size itself.

void double_free(void* ptr, const char* file, long line)

- Classic double free detector. To accomplish this, I stored a freed_blocks unordered set that stores block ptrs only when they have been freed and have not been malloced into an alive state again. These sets will be explained more in a second.

void free_payload_checker(void* ptr, const char* file, long line) 

- A more in depth analysis of a free that takes place within a specific payload. Accomplishes the exact same task as the out_of_bounds free but provides further analysis into where the free occured within an alive payload.


As mentioned, I used unordered sets to store alive_blocks, freed_blocks, and information on when these things were allocated by storing the file and line. This was need for advanced error reporting. Outside of this, not much changed between the design doc and my final implementation that I was not somewhat expecting. Funny enough, writing down the logic here, I just realized I could remove out_of_bounds_free entirely because free_payload_checker accounts for it's job. Making improvements even now.



# What worked? What did not work?

I struggled most with the logic of ordering failure detectors, and understanding the behavior of freed block ptrs that are returned to an alive state with a new allocation 75% of the time, as specified in the base calls.

The ordering of failure detectors was padantic and honestly just took trial and error. Some tests could signal an error that they were not supposed to, so with each implementation of a new failure checker, I had to go back and fix the other checkers because I had to refine exactly what it was checking. This mostly had to do with my lack of understanding of the reusability of freed heap ptrs, meaning I was forced to use unordered sets in order to seperate the references for checking my logic. I created the freed_blocks set for the double_frees in order to see if we are freeing a still non-returned heap ptr, then I used the allocated_blocks set to check for not allocated and in payload errors. But, the issue was I was using the sets wrong, as I assumed I could just free the blocks and use that information. Yet, I then realized that many of the ptrs could be reused, so I had to account for this logic within my malloc function. I understanding this is confusing, but most of the problems with the errors were small and usually errors with my storing of information logic.


What did work was my understanding of how the memory was being laid out in the heap. I comftorably attacked the metadata and right_guard value with ease, ensuring that the bytes were allocated in proper proportion and how to ensure I am using pointer arithmetic to bound between metadata setting values, allocating total payload, and seting the guard value. I am confident in my understanding of memory and how it is layed out within the heap.



# During your research did you run into any issues, what (if any) topics did you have to research?

Not during research, no. I had to research heap allocations with metadata incorporated but not much else. The research was clear and did not lead my astray. My issues came simply from in the mud C++ implementation of the logic within my head.


# How many hours did this assignment take to complete? (i.e,. 10 hours, 20 hours, etc.)

Around 15 hours. Less than most assignments, I just started late because I had many late hours as a cushion.



# UNIT Test: A description should include the purpose of the test, the input data, the expected output, and a brief explanation of how the test confirms the correctness of the function.


UNIT TEST: If a user were to gain understand of how the metadata is structured, they could wreak havoc on the program, since my implementation up to this point relied on finding the size of an allocated block by using the metadata size as a non const size_t. This means if a user backs up and sets the size of the block in the metadata to something much too large or too small, it will wreak havoc on the error checking logic and will cause segfaults when trying to access ptr locations or data that does not exist.

Input Data:
A ptr with it's size metadata maliciously changed to a large value.

 void* p = malloc(10);
    size_t* size = (size_t*)((char*)p - sizeof(size_t)*2);

    *size = 134243;

    //will cause seg faults and logic errors if malicious activity not accounted for
    free(p);

Expected Output:

Should have a way to watch block sizes without giving user the ability to change the metadata and ruin logic.

//! alloc count: active          0   total          1   fail          0
//! alloc size:  active          0   total          10   fail          0


This is the expected output because even though the user maliciously attempted to change our metadata, it does not disrupt the normal malloc and free process and shown by the expected output. If it did affect the programs logic, we should have recieved a seg fault when trying to read the value of the write guard while looking for wild write errors.

