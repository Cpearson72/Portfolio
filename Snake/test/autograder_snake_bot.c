#include <assert.h>
#include <curses.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>

#include "../src/common.h"
#include "../src/game.h"
#include "../src/game_setup.h"
#include "../src/mbstrings.h"


// Verbosity of test runner. Overridden via compilation flag
#ifdef VERBOSE
#define VERBOSE 1
#endif
#ifndef VERBOSE
#define VERBOSE 0
#endif


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

void print_game(int* cells, size_t height, size_t width) {
    setlocale(LC_CTYPE, "");
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            char cell = cells[i * width + j];
            if (cell == FLAG_PLAIN_CELL) {
                printf(".");
            } else if (cell == FLAG_SNAKE) {
                printf("S");
            } else if (cell == FLAG_WALL) {
                printf("%lc", (wint_t)0x02588);
            } else if (cell == FLAG_FOOD) {
                printf("O");
            } else {
                printf("?");
            }
        }
        printf("\n");
    }
    printf("\n");
}

// returns 0 if success, or a board decompress error code if failure
int run_test(int** cells_p, size_t* width_p, size_t* height_p, snake_t* snake_p, 
                char* board_rep, unsigned int snake_grows, char* input_string) {
    
                    
    int status = initialize_game(cells_p, width_p, height_p, snake_p, board_rep);

    // return early if error parsing board
    if (status != INIT_SUCCESS) {
        return status;
    }

    int i = 0;
    while (1) {
        if (VERBOSE) {
            printf("Board at time step %d:\n", i);
            print_game(*cells_p, *height_p, *width_p);
        }
        // if we reach the end of the input, the trace is over
        if (g_score == 1 && snake_grows == 0 && strlen(input_string) < 1) {
            break;
        }

        if(g_score == 5 && snake_grows == 1){
            break;
        }

        if(g_score == 100){
            break;
        }




        // Get user input
        enum input_key input = get_best_input(*cells_p, snake_p, *width_p, *height_p);
        //input_string += 1;

        // Update game state
        update(*cells_p, *width_p, *height_p, snake_p, input, snake_grows);

        i += 1;
    }

    return 0;
}

int main(int argc, char **argv) {
    if (getenv("DEBUG")) {
        sleep(1);
    }

    if (argc < 6) {
        printf(
            "Usage: autograder <board_string> <seed> <snake_grows> <key_input>"
            " <consider_name> <pipe>\n");
        exit(EXIT_FAILURE);
    }

    char *board_string = argv[1];
    unsigned int seed = atoi(argv[2]);
    unsigned int snake_grows = atoi(argv[3]);  // Should be 0 or 1
    char *key_input = argv[4];
    unsigned int consider_name = atoi(argv[5]);  // Should be 0 or 1
    FILE *pipe = fdopen(atoi(argv[6]), "w");

    set_seed(seed);
    // if no board string is provided then use the default board by setting
    // null
    if (board_string[0] == '0') {
        board_string = NULL;
    }

    // Run the snake game
    int* cells;
    snake_t snake;
    // default the board to 0x0 so the stencil doesn't crash
    size_t width = 0;
    size_t height = 0;

    int status =
        run_test(&cells, &width, &height, &snake, board_string, snake_grows, key_input);

    if (status != INIT_SUCCESS) {
        char *msg = "";
        if (status == INIT_ERR_BAD_CHAR) {
            msg = "BAD_CHAR";
        } else if (status == INIT_ERR_INCORRECT_DIMENSIONS) {
            msg = "INCORRECT_DIMENSIONS";
        } else if (status == INIT_ERR_WRONG_SNAKE_NUM) {
            msg = "WRONG_SNAKE_NUM";
        }
        fprintf(pipe,
                "{\n"
                "    \"board_error\": \"%s\"\n"
                "}\n",
                msg);
        teardown(cells, &snake);
        exit(EXIT_SUCCESS);
    }

    char *cell_string = (char *)malloc(
        width * height + 1);
    if (cell_string == NULL) {
        fprintf(stderr, "Failed to allocate memory for cell string\n");
        teardown(cells, &snake);
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            char cell = cells[i * width + j];
            char cell_as_char;
            if (cell == FLAG_PLAIN_CELL) {
                cell_as_char = '.';
            } else if (cell == FLAG_SNAKE) {
                cell_as_char = 'S';
            } else if (cell == FLAG_WALL) {
                cell_as_char = 'X';
            } else if (cell == FLAG_FOOD) {
                cell_as_char = 'O';
            } else {
                cell_as_char = '?';
            }
            cell_string[i * width + j] = cell_as_char;
        }
    }
    cell_string[width * height] = '\0';

    if (consider_name) {
        // Test name reading, mbslen
        char name_buf[1000];
        read_name(name_buf);
        size_t name_len = mbslen(name_buf);

        // Write bytes of name_buf into string
        char name_byte_str_buf[2000];
        assert(strlen(name_buf) < 1000);
        for (size_t i = 0; i <= strlen(name_buf); ++i) {
            if (name_buf[i] == 0) {
                break;
            }
            // Write two hex digits at a time, offset by (i * 2) —
            // each written string is three bytes long, so we overwrite the
            // trailing (null) byte with each write, giving us a continuous
            // string of hex values.
            snprintf(name_byte_str_buf + (i * 2), 3, "%02x",
                     (unsigned char)name_buf[i]);
        }

        fprintf(pipe,
                "{\n"
                "    \"game_over\": %d,\n"
                "    \"score\": %d,\n"
                "    \"name\": \"%s\",\n"
                "    \"name_len\": %lu,\n"
                "    \"width\": %lu,\n"
                "    \"height\": %lu,\n"
                "    \"cells\": \"%s\"\n"
                "}\n",
                g_game_over, g_score,
                name_byte_str_buf, name_len, width,
                height, cell_string);
    } else {
        fprintf(pipe,
                "{\n"
                "    \"game_over\": %d,\n"
                "    \"score\": %d,\n"
                "    \"width\": %lu,\n"
                "    \"height\": %lu,\n"
                "    \"cells\": \"%s\"\n"
                "}\n",
                g_game_over, g_score,
                width, height,
                cell_string);
    }

    teardown(cells, &snake);
    free(cell_string);
    fclose(pipe);
    exit(EXIT_SUCCESS);
}
