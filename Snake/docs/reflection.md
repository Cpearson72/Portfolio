# What changed between your design document and the final implementation.

- The entire design doc was thrown out the window in a way. At first I was planning a similar approach to the threads assignment. That means I was most curious about how to manage direct memory when there may be multiple threads vying for it. This of course did not happen, however, the idea of direct memory management gave me the idea to control the snake by letting it be a pointer that traverses over the allocated memory of the board. To do so, I implemented functions that controlled the snake pointer movement over the board with pointer arithmetic, as well as functions that allowed an easier safeguard to prevent the snake from leaving the board and its memory space. Another part I planned for was to update the snake and board individually. This was unecessary of course as I was able to do everything I needed to on the same address spaces contained with the malloced board. This gave greater efficiency.

# What worked? What did not work?

- As mentioned, controlling the snake with pointer arithmetic made my life much easier compared to my initial approach of tracking the row and col of the snakes head. Using the memory space was huge for simplicity in the entire code base. What I struggled most with was freeing memory(memory leaks, double frees, freeing memory that was not malloced). Every
time I introduced a new data representation that required dynamic memory allocation I ran into constant issues with memory frees as listed above. I had to account for the normal success casses of the game as well as fail cases that returned errors. Both contexts present much different free scenarios, as I had to track what piece of memory was active at what time, and if a part of the program errored out, I had to track when to free the memory. This was frustrating and left me confused many times, but It allowed me to think more like a computer and understand what I was managing at what parts of my program.

# During your research did you run into any issues, what (if any) topics did you have to research?

- Not issues I can think of within my research. The topics I had to research was memory frees as mentioned above, pointer arithmetic to allow my snake to traverse over the boards memory, how UNICODE characters are represented as byes, further clarification on make files to properly run my snake_bot.c file, the existing render, linked lists, common, game_setup, files to understand how they work, and how to use extern declerations.

# How many hours did this assignment take to complete? (i.e,. 10 hours, 20 hours, etc.)

- About 25 hours

# UNIT Test: A description should include the purpose of the test, the input data, the expected output, and a brief explanation of how the test confirms the correctness of the function.

- The purpose of this test will be to test snake growth on a default, custom, and faulty board. Throughout you will be able to test snake movement, food functionality upon game score and growth, and fail cases like snake hitting or growing into the wall or itself.

- Please run: ./unit.test.sh