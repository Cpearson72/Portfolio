#define _XOPEN_SOURCE_EXTENDED 1
#include <curses.h>
#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "game.h"
#include "game_over.h"
#include "game_setup.h"
#include "mbstrings.h"
#include "render.h"


/*------------------------------------------------------------------
                    
                        SNAKE BOT CODE!!                                    

--------------------------------------------------------------------*/
#define SNAKE_BOT 

int get_food_index(int* cells, size_t width, size_t height){
    int food_index = -1;
    for(unsigned long i = 0; i < width * height; i++) {
        if(cells[i] == FLAG_FOOD) {
            food_index = i;
            break;
        }
    }
    return food_index;

}
void get_closest_moves_to_food(int *cells, snake_t* snake_p, size_t width, size_t height, enum input_key moves[], int* num_moves) {
    int food_index = get_food_index(cells, width, height);

    int distance = 0;
    int snake_head_index = ((intptr_t)snake_p->snake_head->data - (intptr_t)cells) / sizeof(int);
    if(food_index > snake_head_index){
        distance = food_index - snake_head_index;
    }
    if(snake_head_index > food_index){
        distance = snake_head_index - food_index;
    }

    // Calculate distances for each move
    int distances[4];

    distances[0] = abs(snake_head_index - (int)width - food_index);  // UP
    distances[1] = abs(snake_head_index + (int)width - food_index);   // DOWN
    distances[2] = abs(snake_head_index - 1 - food_index);      // LEFT
    distances[3] = abs(snake_head_index + 1 - food_index);       // RIGHT

    

    enum input_key directions[4] = {INPUT_UP, INPUT_DOWN, INPUT_LEFT, INPUT_RIGHT};
    
    // Sort moves by distance (using bubble sort for simplicity)
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 3-i; j++) {
            if(distances[j] > distances[j+1]) {
                // Swap distances
                int temp_dist = distances[j];
                distances[j] = distances[j+1];
                distances[j+1] = temp_dist;
                
                // Swap corresponding moves
                enum input_key temp_move = directions[j];
                directions[j] = directions[j+1];
                directions[j+1] = temp_move;
            }
        }
    }

    // Copy sorted moves to output array
    *num_moves = 4;  // or however many moves you want to return
    for(int i = 0; i < *num_moves; i++) {
        moves[i] = directions[i];
    }
}


bool safe_input(int* cells, snake_t* snake_p, size_t width, size_t height){

    //get move amount
    int move_amount = get_move_amount(snake_p->snake_dir, width, height);

    //safe move?
    if(!memory_safe(snake_p->snake_head->data, cells, width, height, move_amount)){
        return false;
    }
    if(!good_move(snake_p->snake_head->data, cells, width, height, move_amount)){
        return false;
    }

    //are we moving closer to wall then the food?
    int snake_head_index = ((intptr_t)snake_p->snake_head->data - (intptr_t)cells) / sizeof(int);
    int snake_col = snake_head_index % width;
    int snake_col_after_move = (snake_head_index + move_amount) % width;
    
    int food_index = get_food_index(cells, width, height);
    int food_col = food_index % width;

    //do we move a further col away from food?
    if(abs(snake_col_after_move - food_col) > abs(snake_col - food_col)){
        return false;
    }
    

    return true;

}


enum input_key get_best_input(int* cells, snake_t* snake_p, size_t width, size_t height){

    enum input_key possible_moves[4] = {INPUT_UP, INPUT_DOWN, INPUT_LEFT, INPUT_RIGHT};
    int num_moves;
    get_closest_moves_to_food(cells, snake_p, width, height, possible_moves, &num_moves);

    for(int i = 0; i < num_moves; i++){
        //curr move
        //set snake_dir
        if(possible_moves[i] == INPUT_UP){
            snake_p->snake_dir = 0;
        }
        else if(possible_moves[i] == INPUT_DOWN){
            snake_p->snake_dir = 2;
        }
        else if(possible_moves[i] == INPUT_LEFT){
            snake_p->snake_dir = 3;
        }
        else if(possible_moves[i] == INPUT_RIGHT){
            snake_p->snake_dir = 1;
        }
        if(safe_input(cells, snake_p, width, height)){
            return possible_moves[i];
        }
    }

    //if all fails, game over, just return first input
    return possible_moves[0];

}




//--------------------------------------------------------------------

/** Gets the next input from the user, or returns INPUT_NONE if no input is
 * provided quickly enough.
 */

const char* board_init_status_to_string(enum board_init_status status) {
    switch (status) {
        case INIT_SUCCESS:
            return "INIT_SUCCESS";
        case INIT_ERR_INCORRECT_DIMENSIONS:
            return "INCORRECT_DIMENSIONS";
        case INIT_ERR_WRONG_SNAKE_NUM:
            return "WRONG_SNAKE_NUM";
        case INIT_ERR_BAD_CHAR:
            return "BAD_CHAR";
        case INIT_UNIMPLEMENTED:
            return "INIT_UNIMPLEMENTED";
        default:
            return "UNKNOWN_STATUS";
    }
}


/** Helper function that procs the GAME OVER screen and final key prompt.
 * `snake_p` is not needed until Part 2!
 */
void end_game(int* cells, size_t width, size_t height, snake_t* snake_p) {
    // Game over!

    // Free any memory we've taken
    teardown(cells, snake_p);

    // ****************** UNCOMMENT THIS CODE IN PART 2B ***********************
    
    // Render final GAME OVER PRESS ANY KEY TO EXIT screen
    render_game_over(width, height);
    usleep(1000 * 1000);  // 1000ms
    cbreak(); // Leave halfdelay mode
    getch();
    

    // tell ncurses that we're done
    endwin();
}

int main(int argc, char** argv) {
    // Main program function — this is what gets called when you run the
    // generated executable file from the command line!

    // Board data
    size_t width;   // the width of the board.
    size_t height;  // the height of the board.
    int* cells = NULL;    // a pointer to the first integer in an array of integers
                    // representing each board cell.

    // snake data (only used in part 2!)
    snake_t snake;   // your snake struct. (not used until part 2!)
    int snake_grows;  // 1 if snake should grow, 0 otherwise.

    enum board_init_status status;

    // initialize board from command line arguments
    switch (argc) {
        case (2):
            snake_grows = atoi(argv[1]);
            if (snake_grows != 1 && snake_grows != 0) {
                printf(
                    "snake_grows must be either 1 (grows) or 0 (does not "
                    "grow)\n");
                return 0;
            }
            status = initialize_game(&cells, &width, &height, &snake, NULL);
            break;
        case (3):
            snake_grows = atoi(argv[1]);
            if (snake_grows != 1 && snake_grows != 0) {
                printf(
                    "snake_grows must be either 1 (grows) or 0 (does not "
                    "grow)\n");
                return 0;
            } else if (*argv[2] == '\0') {
                status = initialize_game(&cells, &width, &height, &snake, NULL);
                break;
            }
            status = initialize_game(&cells, &width, &height, &snake, argv[2]);
            break;
        case (1):
        default:
            printf("usage: snake <GROWS: 0|1> [BOARD STRING]\n");
            return 0;
    }

    // ----------- DO NOT MODIFY ANYTHING IN `main` ABOVE THIS LINE -----------

    // Check validity of the board before rendering!
    // TODO: Implement (in Part 1C)
    // if ( ? board is not valid ? ) { return EXIT_FAILURE; }
    if(status != INIT_SUCCESS){
        char* status_string = (char*)board_init_status_to_string(status);
        printf("%s\n",status_string);
        return status;
    }

    // Read in the player's name & save its name and length
    // TODO: Implement (in Part 2B)
    char name_buffer[1000];
    read_name(name_buffer);
    // ? save name_buffer ?
    g_name = name_buffer;
    // ? save mbslen(name_buffer) ?
    g_name_len = mbslen(name_buffer);

    // TODO: Remove this message, uncomment the code below this message
    //       and implement Part 1A here.
    /*printf(
        "             ____   \n"
        "Hello       / . .\\ \n"
        "CSC412      \\  ---<\n"
        "student!     \\  /  \n"
        "   __________/ /    \n"
        "-=:___________/\n");
        */

    initialize_window(width, height);
    // TODO: implement the game loop here (Part 1A)!
    while(g_game_over != 1 && g_score < 10){
        //wait for user input. this will update snake dir
        usleep(100000);
        
        enum input_key input = get_best_input(cells, &snake, width, height);
        
        update(cells, width, height, &snake, input, snake_grows);

        render_game(cells, width, height);
    }
    end_game(cells, width, height, &snake);
}
