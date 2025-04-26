Conor Pearson
CSC412
Spring 2025

## Requirements: 

- Track information about dynamic memory allocation.
- Catch common memory programming errors (e.g., freeing the same block twice, trying to allocate too much memory etc).
- Detect a write outside a dynamically allocated block (e.g., writing 65 bytes into a 64-byte piece of memory).
- Other more general programming errors.



# Describe in detail which parts of the assignments you will attempt in what order. This is your plan!

- I will follow the assignment as intstructed, in the order of defining and updating statistics, integer overflow protection(accounting for different arithmatic operations), invalid free and double-free detection, boundary write error detection, memory leak reporting, other advanced reports, and lastly a performance check with C++ integration.

- As of 4/13, I am planning on going for the extra credit by programming in C. This means I must first figure out what I am lacking in C that is present in C++ and understand how to make up for this. Next, I must figure how how I will store metadata with my allocated blocks of memory and how I will read and write to the memory without the metadata interfering. Lastly, I must check to see the call heiarchy of dmalloc and dfree to base_malloc and base_free to understand what is different about these calls. From here, I can begin to tackle the steps above.

# Describe at least two areas you think will be difficult or time consuming.

- Integer overflow and consequences of arithmatic operation. I am worried about how I will set the boundaries in my allocated memory/stack defined memory to ensure I am only comparing the sequence of bits according to the arithmatic logic. My questions for this are, how are stack/static/dynamic variables defined in memory exactly, including there wrapping. Do all include a wrapping with some meta data? How differently must my checker be for the arithmatic operations? How do these operations result in different bitwise behavior?

- Memory organization. I feel strongly about my ability to solve the other tests, but the organization of memory I will use to compute the solutions is scary. Are there strict rules to where and when I can allocate memory? Does the OS step in and try to take control of the memory at any point? What failures could occur? How can I dynamically ensure I have little padding without setting a predefined global variable of last call?

# Then, describe what you need to research to solve these problems.

- I must touch up on integer representation in memory. While I have a strong understanding on bit addition and bit multiplication, is there ever any extra room/overhead required to perform these calculations within memory space? I understand the math operations are occuring in the ALU's, but is there some communication protocol between the ALU and the memory space I must be aware of?

- As briefly mentioned, the rules on storing and manipulating memory. Does C/C++ and Linux have different behvaiors on when they require memory manipulation and will my personal implementations of malloc and free require me to abound to those rules? How can I ensure I am abstractly triggering and stopping certain errors without the errors specifically occuring and crashing the system? Does the context of the smaller codebase change the approach I would need for a large codebase?

# Provide a rough outline of the functions, data structures, and/or comments describing what you think you will need.

void update_statistics(size, free/allocate):

- Use the size of the free/malloc block to update global variables like total_size, heap_min, heap_max, etc. Free/allocate will be a bool used to determine if I should be subtracting or adding to these variables. The other variables such as fail_size, n_total, nactive can be passively updated by the presence of certain function calls.

bool integer_overflow_protected(size, *mem):

- use size in bytes to determine if the byte allocation will result in an integer overflow at the specified memory to be allocated to.

- I can compute the size as a decimal or set a predefined variable that will define max size to allocate. If it is less than that size, we can determine if there is enough available space to allocate.

bool invalid_free(*mem_ptr, size):

- will traverse a block of previously allocated memory and examine the meta data. This meta data, I believe, will store the life time and current statistic on this allocated block. From there I can examine if this block has already been allocated or not.

bool boundary_write_error(*mem_ptr, size):

- first determine boundaries of said memory block and then use the size of the read/write command to determine if we would go outside these boundaries. I am unsure if the location of the read/write will have a major effect, but if it does, I can pass another argument that returns a pointer to where the memory will exactly be read/written to.

void memory_leak_report(*heap_start):

- start at beginning of heap and examine all meta data to ensure dynamically allocated memory was freed. If not, simply log the memory location as a memory leak.