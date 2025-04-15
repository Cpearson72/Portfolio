#include "game_setup.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Some handy dandy macros for decompression
#define E_CAP_HEX 0x45
#define E_LOW_HEX 0x65
#define S_CAP_HEX 0x53
#define S_LOW_HEX 0x73
#define W_CAP_HEX 0x57
#define W_LOW_HEX 0x77
#define DIGIT_START 0x30
#define DIGIT_END 0x39

/** Initializes the board with walls around the edge of the board.
 *
 * Modifies values pointed to by cells_p, width_p, and height_p and initializes
 * cells array to reflect this default board.
 *
 * Returns INIT_SUCCESS to indicate that it was successful.
 *
 * Arguments:
 *  - cells_p: a pointer to a memory location where a pointer to the first
 *             element in a newly initialized array of cells should be stored.
 *  - width_p: a pointer to a memory location where the newly initialized
 *             width should be stored.
 *  - height_p: a pointer to a memory location where the newly initialized
 *              height should be stored.
 */

bool default_board = false;

enum board_init_status initialize_default_board(int** cells_p, size_t* width_p,
                                                size_t* height_p, snake_t* snake_p) {
    default_board = true;
    *width_p = 20;
    *height_p = 10;
    //20 is collumn size and 10 is row size
    int* cells = malloc(20 * 10 * sizeof(int));
    *cells_p = cells;

    //linked list
    snake_p->snake_head = malloc(sizeof(node_t));
    snake_p->snake_head->next = NULL;
    snake_p->snake_head->prev = NULL;
    
    
    snake_malloced = 1;
    //2D array represented as 1D
    for (int i = 0; i < 20 * 10; i++) {
        cells[i] = FLAG_PLAIN_CELL;
    }

    // Set edge cells!
    // Top and bottom edges:
    for (int i = 0; i < 20; ++i) {
        cells[i] = FLAG_WALL;
        cells[i + (20 * (10 - 1))] = FLAG_WALL;
    }
    // Left and right edges:
    for (int i = 0; i < 10; ++i) {
        cells[i * 20] = FLAG_WALL;
        cells[i * 20 + 20 - 1] = FLAG_WALL;
    }

    // Add snake
    cells[20 * 2 + 2] = FLAG_SNAKE;

    //place food
    place_food(cells, 20, 10);

    //set snake head to that memory address
    snake_p->snake_head->data = &cells[20 * 2 + 2];
    snake_p->snake_dir = 1;
    snake_p->snake_head->dir = 1;

    return INIT_SUCCESS;
}

/** Initialize variables relevant to the game board.
 * Arguments:
 *  - cells_p: a pointer to a memory location where a pointer to the first
 *             element in a newly initialized array of cells should be stored.
 *  - width_p: a pointer to a memory location where the newly initialized
 *             width should be stored.
 *  - height_p: a pointer to a memory location where the newly initialized
 *              height should be stored.
 *  - snake_p: a pointer to your snake struct (not used until part 2!)
 *  - board_rep: a string representing the initial board. May be NULL for
 * default board.
 */
enum board_init_status initialize_game(int** cells_p, size_t* width_p,
                                       size_t* height_p, snake_t* snake_p,
                                       char* board_rep) {
    // TODO: implement!
    //If default
    snake_malloced = 0;
    if(board_rep == NULL){
        return initialize_default_board(cells_p, width_p, height_p, snake_p);
    }

    return decompress_board_str(cells_p, width_p, height_p, snake_p, board_rep);

}

/** Takes in a string `compressed` and initializes values pointed to by
 * cells_p, width_p, and height_p accordingly. Arguments:
 *      - cells_p: a pointer to the pointer representing the cells array
 *                 that we would like to initialize.
 *      - width_p: a pointer to the width variable we'd like to initialize.
 *      - height_p: a pointer to the height variable we'd like to initialize.
 *      - snake_p: a pointer to your snake struct (not used until part 2!)
 *      - compressed: a string that contains the representation of the board.
 * Note: We assume that the string will be of the following form:
 * B24x80|E5W2E73|E5W2S1E72... To read it, we scan the string row-by-row
 * (delineated by the `|` character), and read out a letter (E, S or W) a number
 * of times dictated by the number that follows the letter.
 */

bool handle_board_dimension(char* dimension, size_t* width_p, size_t* height_p){

    if(dimension[0] != 'B'){
        return false;
    }

    //loop through dimension until we hit x
    long unsigned int i = 1;
    int height = 0;
    while(isdigit(dimension[i])){
        height = (height * 10) + (dimension[i] - '0');
        i++;
    }

    //check if x
    if(dimension[i] != 'x'){
        return false;
    }

    i++;
    int width = 0;
    while(i < strlen(dimension) && isdigit(dimension[i])){
        width = (width * 10) + (dimension[i] - '0');
        i++;
    }

    //set width and height
    *width_p = width;
    *height_p = height;

    return true;

}

void grab_char(char* curr_char, char* row, long unsigned int* index, int* num){

    //grab char from first index
    *curr_char = row[*index];

    *index = *index + 1;

    //loop until row[index] is a non num char
    while(isdigit(row[*index])){
        *num = (*num * 10) + (row[*index] - '0');
        *index = *index + 1;
    }

    /*now we have the char and the num as well as setting
    the index to the next char.
    */
    
}

bool is_good_char(char c){
    if((c == 'E') | (c == 'S') | (c == 'W')){
        return true;
    }
    return false;
}

//this function will assume cells has already had it's memory allocated
bool handle_board_row_rep(int** cells_p, char* row, char** row_rep, size_t* width_p, size_t* height_p){
    
    
    //var for processing row
    char* curr_char = malloc(sizeof(char));
    int* num = malloc(sizeof(int));
    long unsigned int* index = malloc(sizeof(long unsigned int));
    *index = 0;
    *num = 0;

    while(*index < strlen(row)){
        //reset num
        *num = 0;

        grab_char(curr_char, row, index, num);

        if(!is_good_char(*curr_char)){
            free(curr_char);
            free(num);
            free(index);

            return false;
        }

        char* temp = malloc((sizeof(char)) * (*num + 1));
        for(int i = 0; i < *num; i++){
            temp[i] = *curr_char;
        }

        //null term temp
        temp[*num] = '\0';

        // Initial allocation if row_rep is NULL
        if(*row_rep == NULL) {
            *row_rep = malloc(sizeof(char) * (*num + 1));
            (*row_rep)[0] = '\0';  // Initialize as empty string
        }
        else {
            // Reallocate with space for existing + new + null terminator
            char* temp_ptr = realloc(*row_rep, sizeof(char) * (strlen(*row_rep) + *num + 1));
            if(temp_ptr == NULL) {
                free(temp);
                return false;  // Reallocation failed
            }
            *row_rep = temp_ptr;
        }

        //set row_rep to temp
        strcat(*row_rep, temp);
        free(temp);
    


    }

    //free
    free(curr_char);
    free(num);
    free(index);

    return true;

}

enum board_init_status board_set_up(int** cells_p, size_t width, size_t height, char* board_rep, snake_t* snake_p){
    //allocate memory for cells
    int* cells = malloc(width * height * sizeof(int));
    *cells_p = cells;
    snake_malloced = 1;

    snake_p->snake_head = NULL;
    

    bool snake_found = false;
    //write to cells
    for(long unsigned int i = 0; i < (width * height); i++){
        if(board_rep[i] == 'W'){
            cells[i] = FLAG_WALL;
        }
        else if(board_rep[i] == 'S'){
            if(!snake_found){
                cells[i] = FLAG_SNAKE;
                snake_found = true;
                //set snake head
                snake_p->snake_head = malloc(sizeof(node_t));
                snake_p->snake_head->next = NULL;
                snake_p->snake_head->prev = NULL;
                snake_p->snake_head->data = &cells[i];
                snake_p->snake_dir = 1;
                snake_p->snake_head->dir = 1;
            }
            else{
                free(board_rep);
                return INIT_ERR_WRONG_SNAKE_NUM;
            }
        }
        else if(board_rep[i] == 'E'){
            cells[i] = FLAG_PLAIN_CELL;
        }
        else{
            return INIT_ERR_BAD_CHAR;
        }
    }

    free(board_rep);
    if(!snake_found){
        return INIT_ERR_WRONG_SNAKE_NUM;
    }
    place_food(cells, width, height);

    return INIT_SUCCESS;

}

enum board_init_status decompress_board_str(int** cells_p, size_t* width_p,
                                            size_t* height_p, snake_t* snake_p,
                                            char* compressed) {
    // TODO: implement!
    //set board_rep string
    char* dimension = strtok(compressed, "|");
    char* decompressed = NULL;
    
    //For first token that includes board dimension.
    //This function set's the width and height pointers
    if(!handle_board_dimension(dimension, width_p, height_p)){
        return INIT_ERR_BAD_CHAR;
    }

    //grab current row
    char* row = strtok(NULL, "|");
    long unsigned int row_count = 0;
    while(row != NULL){
        char* row_rep = NULL;
        if(!handle_board_row_rep(cells_p, row, &row_rep, width_p, height_p)){
            if(decompressed != NULL){
                free(decompressed);
            }
            if(row_rep != NULL){
                free(row_rep);
            }
            return INIT_ERR_BAD_CHAR;
        }

        //allocate memory to decompressed
        if(decompressed == NULL){
            decompressed = malloc(sizeof(char) * (strlen(row_rep) + 1));
            decompressed[0] = '\0';
        }
        else{
            decompressed = realloc(decompressed, sizeof(char) * (strlen(decompressed) + strlen(row_rep) + 1));
        }

        //write to decompressed
        strcat(decompressed, row_rep);

        if(strlen(row_rep) > *width_p || strlen(row_rep) < *width_p){
            free(row_rep);
            free(decompressed);
            return INIT_ERR_INCORRECT_DIMENSIONS;
        }

        free(row_rep);

        row = strtok(NULL, "|");

        row_count += 1;

        if(row_count > *height_p){
            free(decompressed);
            return INIT_ERR_INCORRECT_DIMENSIONS;
        }
    }

    //is decompressed larger than width * height?
    if(decompressed != NULL){
        if(strlen(decompressed) > (*width_p * *height_p)){
            free(decompressed);
            return INIT_ERR_INCORRECT_DIMENSIONS;
        }
    
        else if(strlen(decompressed) < (*width_p * *height_p)){
            free(decompressed);
            return INIT_ERR_INCORRECT_DIMENSIONS;
    }
}
    else{
        free(decompressed);
        return INIT_ERR_INCORRECT_DIMENSIONS;
    }

    
    return board_set_up(cells_p, *width_p, *height_p, decompressed, snake_p);
}
                                            
