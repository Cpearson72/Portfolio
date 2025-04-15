# Describe in detail which parts of the assignments you will attempt in what order. This is your plan!

- I will follow a rough estimate of the assignment instructions.

- 1. Board and Snake initialization:

As specified, the board and snake will be stored in a more abstract way. Rather than reserving an array or matrix for both, the board and snake will have a reserved set of memory that will be manipulated as if it was an array. This direct memory access, I assume, provides efficiency for a game that requires immediate input reponse. To achieve this I will set a function for both. Initialize_Board and Initialize_Snake:

## Function 1:

enum board_init_status
initialize_default_board(int** cells, size_t* width, size_t* height){
    //reserve memory space for the cells using width and height as the reference for where to end allocation. This will be accomplished through malloc and other memory allocation related calls.

    //Test the memory space, ensuring each cell has a dedicated memory space, as well as the num of cells fitting the space required by width and height. This part will require research for built in functions that allow for this testing.

}

## Function 2:

initialize_snake(int** cells){
    //Allocate memory space for the snake using malloc and other memory allocation related calls. Enough memory must be allocated where the snake can fit into any non-wall cell on the board. The data structure stored at each memory space will for now be seen as a struct, which can include flags is_head, is_tail. This will help for clarifcation in the future functions.

    //Test the memory space, ensuring proper sequence of memory. This part will also require research in how to test.

}

I will outline the rest of my proposed functions at the end of this design document.

# Describe at least two areas you think will be difficult or time consuming.

- Area 1:

Active parallel thread and memory management. I believe I will implement threads that will individually control the snake, board updates, food, etc. However, the nature of direct memory management could make this more difficult than I am picturing in my head. Before we were able to use mutexes to control an abstract resource of ink, but memory is not an abstract resource and likely will have immediate weird and unusual behaviors in my code that I have not run into before.

- Area 2:

Board manipulation. As the board is not stored in a matrix like the last assignment, understanding its active manipulation will be difficult. When the snake moves in a particular direction, I need to manage proper manipulation of the cell it is entering. Maybe I can make helper functions that can track the current memory address of the head and then implement a system for understanding how UP, LEFT, DOWN, or RIGHT are portrayed as jumps between this memory space.

# Then, describe what you need to research to solve these problems.

Primarily active memory management. How is memory initialized when you allocate space? Is it stored all in consecutive order? Are there other processes or markers I am unaware of that will store space between some of the address spaces? Will the address spaces be stored backwards? Will I run into memory corruption? Does alteriting one memory space have potential affects on the neighboring spaces?

My knowledge of memory spaces is not strong, I will admit, and all these questions will need to be answered until I can implement a real solution to the memory manipulation of the grid and snake.

# Provide a rough outline of the functions, data structures, and/or comments describing what you think you will need.

Here are the rest of the functions and their data structures that I am currently brainstorming with.

## Function 3:

updateBoard(int** grid, size_t* width, size_t* height){
    //go through memory and update board based on the behavior of the snake, i.e if it ate food and grew in size, the direction it just turned, and if a snake cell is leaving or just entered a grid cell. I am assuming I can use a key of 0,1,2,3 for the memory spaces of the grid. 0 is no wall or snake, 1 is no wall and snake present, 2 is wall and no snake present, 3 is wall and snake present. If 0, print normal, 1 print with snake, 2 print wall, 3 end game.

}

## Function 4:

keyListener(){
    //This will be managed by a key listener thread that will watch for keyboard inputs. If an input is present using the read() function using the FILEN0 file descriptor, then the update snake function will be called with that particular key as the argument given.

    //This function will not be difficult and should follow a simple premise of just waiting for a key press.

}

## Function 5:

updateSnake(keyPress, int** snake){
    //move the snake position using a helper function. the head of the snake may be a struct that will store the address of the cell of the grid it is in. Call a helper function(that will not be defined right now), that will change/jump the memory address in the correct manner within the grid. This helper function can also manipulate the current value of that new memory address.

    //Call if_on_food function to determine if growth should be applied. If food is present at this memory address, use a helper function grow_snake(int** snake) that traverses to the memory address of the snake tail and then increments the size by shifting the tail to the next empty memory address then setting that as the new tail. We will have a way of determining a valid snake memory address vs a reserved snake memory address.

}

Most of the other functions will be helper functions that are used by these main functions, but it is hard to determine exactly what these will be until a prototype starts to be built. Not only must I plan the design of these functions, I must plan for the thread utilization being used by the board, snake, food, and key listener. Any helper function must call or signal to the thread itself rather than an arbitrary function call.