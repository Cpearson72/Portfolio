#define _XOPEN_SOURCE_EXTENDED 1
#include <curses.h>
#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "game.h"
#include "game_over.h"
#include "game_setup.h"
#include "mbstrings.h"
#include "render.h"

/** Gets the next input from the user, or returns INPUT_NONE if no input is
 * provided quickly enough.
 */
enum input_key get_input() {
    /* DO NOT MODIFY THIS FUNCTION */
    int input = getch();

    if (input == KEY_UP) {
        return INPUT_UP;
    } else if (input == KEY_DOWN) {
        return INPUT_DOWN;
    } else if (input == KEY_LEFT) {
        return INPUT_LEFT;
    } else if (input == KEY_RIGHT) {
        return INPUT_RIGHT;
    } else {
        // if the input isn't an arrow key, we treat it as no input (could add
        // other keys in if we want other commands, like 'pause' or 'quit')
        return INPUT_NONE;
    }
    /* DO NOT MODIFY THIS FUNCTION */
}

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
    int tester = 0;
    while(g_game_over != 1){
        //wait for user input. this will update snake dir
        usleep(100000);
        enum input_key input = get_input();
        /*if(tester > 2){
            input = INPUT_DOWN;
        }*/
        update(cells, width, height, &snake, input, snake_grows);

        render_game(cells, width, height);
        tester++;
    }
    end_game(cells, width, height, &snake);
}
