# What was implemented?

I implemented a snake bot that will find food on it's own, while avoiding walls, it's own body, and while taking the shortest path towards the food. To accomplish this I created these functions:

## int get_food_index(int* cells, size_t width, size_t height);

- This function simply returns the index of the food cell. This is a helper function to be called by the next three functions for the sake of finding the move which brings our snake closest to the food.

## void get_closest_moves_to_food(int *cells, snake_t* snake_p, size_t width, size_t height, enum input_key moves[], int* num_moves)

- Ranks all the inputs based off the lowest distance from snake head to food after the input. So if input_up gives a distance of 50, down gives 30, left gives 40, and right gives 10, it will return the list in the order of [RIGHT, DOWN, LEFT, UP].

## bool safe_input(int* cells, snake_t* snake_p, size_t width, size_t height)

- Takes the input we are trying to test, often the best input first based of new distance, and then checks if it is a safe memory be reusing my "bool memory_safe"(checks if it leaves our memory space) and "bool good_move"(checks to see if it hits wall or snake body) functions from the main assignment. If it pasts the tests, return true, and false otherwise.

## enum input_key get_best_input(int* cells, snake_t* snake_p, size_t width, size_t height)

- Calls the necessary functions above, getting the list of moves from the get_cloest_moves_to_food function. It then cycles through the moves from best to worst until safe_input passes, and then we return the move that had passed.


# Small changes

- Before we test the bot, I must include that I made the snake body a solid color and I made the food an apple. To achieve this all I had to do was change the code in render to these lines.

Snake:

if (cells[i] & FLAG_SNAKE) {
            cchar_t c;
            setcchar(&c, L"■", WA_NORMAL, COLOR_SNAKE, NULL);
            ADDW(i / width, i % width, &c);
        }


Food:

else if (cells[i] & FLAG_FOOD) {
            cchar_t c;
            setcchar(&c, L"🍎", WA_NORMAL, COLOR_FOOD, NULL);
            ADDW(i / width, i % width, &c);            
        }


- Now it is time to test for yourself. Run the bash script below!

# TEST: ./snake_bot.sh


   