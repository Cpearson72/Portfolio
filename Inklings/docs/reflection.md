# What changed between your design document and the final implementation.

- The entire design document was thrown out the window. I did not reference it even for a second. Everything I made was on my own.

# What worked? What did not work?

- What worked was my manipulation of the grid and my understanding of the ascii_art. What did not work was my mutex and log function implementation. The issue with the mutex's was deadlocks. I employed mutex's within each function and then employed unlocks, not knowing this was not needed with a lock_guard since it would unlock once the scope left. This resulted in a number of deadlocks, specifically when trying to acquire ink. I had a mutex on the check enough ink, acquire ink, and while moving. This of course led to a number of deadlocks, where I decided to only implement them into my moving function, meaning that the process of checking and acquiring ink was done under the scope of that inklings particular color lock. This made the use of a mutex more efficient and simpler. Lastly, my issues with the log function had to do with the speed of the threads, or better yet, the sleep time of the inklings. At first they were beyond slow, resulting in a number of inklings not ever moving when there was about twenty threads as many of them were vying for the same research.

# During your research did you run into any issues, what (if any) topics did you have to research?

- The only topic I had to research was the behavior of a mutex, with how they unlock once out of scope. I also had to research the behavior of detaching threads, which made my program easier to manage. Other than that, it was nothing but problem solving.

# How many hours did this assignment take to complete? (i.e,. 10 hours, 20 hours, etc.)

- 22-25 hours easily

# UNIT TEST:

- I am providing a bash script that will run the program with a number of arguments, including no arguments. I will provide instructions on how to test the functionality of the program as well as understand its behavior.

- The expected output is the proper functionality of the keyboard inputs and inkling behavior upon the grid.

- please run ./unit_test.sh

