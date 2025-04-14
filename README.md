# Portfolio

## What am I looking at?

- This portfolio consists of C,C++,and Python projects that display my understanding of low-level coding concepts like memory management, and multi-threading, as well as more abstract and higher-level concepts like algorithms and complex data structures. I have chosen a select few projects to portray my understanding of each, starting with Snake, Inklings, which show memory management usage and multi-threading respectively. Next, I have included the projects convex hull, image seam carving, and compression, which shows my ability to split a large problem into a series of tiney ones, better known as the recurrence relation. I will list the projects in order and explain how to compile them, with the C files compling with make and python/c++ files being compiled in the cli.


## Snake

Short Overview:
- Snake is a fully functional recreation of the orginal snake game. Created in C, the snake and it's customizable board(RLE encoding), are made and manipulated using dynamically allocated memory through malloc functions. Snake movement and board updates all occur on the same block of allocated memory, with the snake using pointer arithmetic to traverse over the board. To avoid memory leaks, double frees, and use after frees, there are a number of edge cases that require complex monitoring of the allocated memory. One such example is freeing memory upon a new snake-tail colliding with a wall after the snake has consumed an apple. The issue here is that if the new tail becomes linked to the linked-list representing the snake, our future free call at the conclusion of our program will result in a double free.

Skills Shown:
- My understanding of memory leaks, double frees, and use after frees, and how to avoid them through memory management.

Compilation Instructions:
- Download files and use the command, "make" to compile all files into the snake executable.


## Inklings

Short Overview:
- Inklings is a multi-threaded C application where threads are represented as inklings moving on a grid. Each inkling has a potential color of red, green, or blue, with each color having a limited resource tank. With multiple inklings of the same color, the threads/inklings are vying for the same resource, meaning I had to implement thread synchronization with the use of mutexes. This was a complex task where the mutexes had to be used in a large number of contexes, without being able to ever overlap, in order to avoid dead locks.

Skills Shown:
- Resource management and thread synchronization.

Compilation Instructions:
- Use the command, "make".
