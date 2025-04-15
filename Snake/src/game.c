#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "game_setup.h"

#include "linked_list.h"
#include "mbstrings.h"

/** Updates the game by a single step, and modifies the game information
 * accordingly. Arguments:
 *  - cells: a pointer to the first integer in an array of integers representing
 *    each board cell.
 *  - width: width of the board.
 *  - height: height of the board.
 *  - snake_p: pointer to your snake struct (not used until part 2!)
 *  - input: the next input.
 *  - growing: 0 if the snake does not grow on eating, 1 if it does.
 */

int grow = 0;
void update(int* cells, size_t width, size_t height, snake_t* snake_p,
            enum input_key input, int growing) {
    // `update` should update the board, your snake's data, and global
    // variables representing game information to reflect new state. If in the
    // updated position, the snake runs into a wall or itself, it will not move
    // and global variable g_game_over will be 1. Otherwise, it will be moved
    // to the new position. If the snake eats food, the game score (`g_score`)
    // increases by 1. This function assumes that the board is surrounded by
    // walls, so it does not handle the case where a snake runs off the board.

    // TODO: implement!
    grow = growing;

    //if game over exit
    if(g_game_over == 1){
        return;
    }

    //validate input
    int curr_dir = snake_p->snake_dir;
    snake_p->snake_dir = validate_input(input, snake_p->snake_dir, growing);
    snake_p->snake_head->dir = snake_p->snake_dir;

    int moveamount = get_move_amount(snake_p->snake_dir, width, height);
    
    //check for memory saftey and if hits wall
    if(!memory_safe(snake_p->snake_head->data, cells, width, height, moveamount)){
        g_game_over = 1;
        return;
    }

    if(!good_move(snake_p->snake_head->data, cells, width, height, moveamount)){
        g_game_over = 1;
        return;
    }

    //okay so it is a safe move, let's update the board
    if(growing == 1){
        update_board_with_growth(cells, snake_p, moveamount, width, height, curr_dir);
    }
    else{

    //implement growth and non growth functions
    int snake_index = ((int*)snake_p->snake_head->data) - cells;
    cells[snake_index] = FLAG_PLAIN_CELL;


    int new_snake_index = snake_index + moveamount;
    //food?
    if(eat_food(snake_p->snake_head->data, cells, new_snake_index)){
        g_score++;
        if(growing == 1){
            //implement after finishing
        }
        place_food(cells, width, height);
    }
    
    snake_p->snake_head->data = cells + new_snake_index;
    cells[new_snake_index] = FLAG_SNAKE;

}


}


void update_board_with_growth(int* cells, snake_t* snake_p, int moveamount, size_t width, size_t height, int curr_dir){

    //loop through nodes
    snake_t current;
    current.snake_head = snake_p->snake_head;
    current.snake_dir = snake_p->snake_dir;


    int ate_food = 0;

    int new_index = 0;
    int prev_index = -1;

    while(current.snake_head != NULL){
        //get curr_index of the part of the body and then move it first
        int curr_index = ((int*)current.snake_head->data) - cells;
        
        if(current.snake_head == snake_p->snake_head){
            //get new index
            new_index = curr_index + get_move_amount(snake_p->snake_head->dir, width, height);
            //if head and hit food
            
            if(eat_food(current.snake_head->data, cells, new_index)){
                ate_food = 1;
            }
        

        //set cells
        cells[curr_index] = FLAG_PLAIN_CELL;
        cells[new_index] = FLAG_SNAKE;

        //set mem address of curr
        current.snake_head->data = cells + new_index;

        }

       else{
            cells[curr_index] = FLAG_PLAIN_CELL;
            cells[prev_index] = FLAG_SNAKE;
            current.snake_head->data = cells + prev_index;
            current.snake_head->dir = current.snake_head->prev->dir;
       }

       prev_index = curr_index;
       current.snake_head = current.snake_head->next;

    }


    //now the current snake has moved on the board. now did we eat food?

    if(ate_food == 1){
        g_score++;

        //lets add a tail
        node_t* curr_tail = get_last(snake_p->snake_head);
        int curr_tail_index = ((int*)curr_tail->data) - cells;

        //make a new tail
        node_t* new_tail = malloc(sizeof(node_t));
        
        int tail_location = where_to_place_tail(curr_tail);
        int new_tail_index = curr_tail_index + tail_location;

        //if these two pass we are safe to place the tail
        if(!memory_safe(curr_tail->data, cells, width, height, tail_location)){
            free(new_tail);
            g_game_over = 1;
            return;
        }
        if(!good_move(curr_tail->data, cells, width, height, tail_location)){
            free(new_tail);
            g_game_over = 1;
            return;
        }

        cells[new_tail_index] = FLAG_SNAKE;
        //connect new tail to curr tail
        new_tail->data = cells + new_tail_index;
        new_tail->dir = curr_tail->dir;
        new_tail->prev = curr_tail;
        new_tail->next = NULL;

        curr_tail->next = new_tail;

        place_food(cells, width, height);
        
    }

   
}

int where_to_place_tail(node_t* tail){
    if(tail->dir == 0){
        return +20;
    }
    if(tail->dir == 1){
        return -1;
    }
    if(tail->dir == 2){
        return -20;
    }
    if(tail->dir == 3){
        return +1;
    }
    return 0;
}

/** Sets a random space on the given board to food.
 * Arguments:
 *  - cells: a pointer to the first integer in an array of integers representing
 *    each board cell.
 *  - width: the width of the board
 *  - height: the height of the board
 */
void place_food(int* cells, size_t width, size_t height) {
    /* DO NOT MODIFY THIS FUNCTION */
    unsigned food_index = generate_index(width * height);
    if (*(cells + food_index) == FLAG_PLAIN_CELL) {
        *(cells + food_index) = FLAG_FOOD;
    } else {
        place_food(cells, width, height);
    }
    /* DO NOT MODIFY THIS FUNCTION */
}

/** Prompts the user for their name and saves it in the given buffer.
 * Arguments:
 *  - `write_into`: a pointer to the buffer to be written into.
 */
void read_name(char* write_into) {
    // TODO: implement! (remove the call to strcpy once you begin your
    // implementation)

    //print prompt "Name > "
    printf("Name > ");
    fflush(0);
    //use read system call to read in name
    ssize_t bytes_read;
    bytes_read = read(0, write_into, 1000);

    //if 0 characters enter print "Name > "
    if(bytes_read <= 1){
        printf("Name Invalid: must be longer than 0 characters");
        read_name(write_into);
    }

    write_into[bytes_read] = '\0';
    //strcpy(write_into, "placeholder");
}

/** Cleans up on game over — should free any allocated memory so that the
 * LeakSanitizer doesn't complain.
 * Arguments:
 *  - cells: a pointer to the first integer in an array of integers representing
 *    each board cell.
 *  - snake_p: a pointer to your snake struct. (not needed until part 2)
 */
void teardown(int* cells, snake_t* snake_p) {
    // TODO: implement!
    if(cells == NULL){
        cells = malloc(sizeof(int));
        if(cells != NULL){
            free(cells);
    }
    }else if(default_board == true){
       free(cells);
    }

    // First free the snake nodes
    if(grow == 1){
    if (snake_p->snake_head != NULL) {
        node_t* current = snake_p->snake_head;
        
        while (current != NULL) {
            node_t* to_free = current;
            current = current->next;  // Move to next node before freeing
            free(to_free);           // Free the previous node
        }
        snake_p->snake_head = NULL;  // Clear the head pointer
    }
}
    
}   

    




bool memory_safe(int* snake_head, int* cells, size_t width, size_t height, int move_amount){
    //for comparison
    long int width_c = width;
    long int height_c = height;
    //get index
    ptrdiff_t index = snake_head - cells;

    //get index after move
    ptrdiff_t index_after_move = index + move_amount;

    //are we out of bounds?
    if(index_after_move < 0 || index_after_move >= width_c * height_c){
        return false;
    }

    return true;

}

bool good_move(int* snake_head, int* cells, size_t width, size_t height, int move_amount){

    //get index
    ptrdiff_t index = snake_head - cells;

    //get index after move
    ptrdiff_t index_after_move = index + move_amount;

    //get value at index after move
    int value_at_index_after_move = *(cells + index_after_move);

    //is it a wall?
    if(value_at_index_after_move == FLAG_WALL){
        return false;
    }

    //is it a snake?
    if(value_at_index_after_move == FLAG_SNAKE){
        return false;
    }

    return true;
}


int get_move_amount(int snake_dir, size_t width, size_t height){
    switch(snake_dir){
        //North
        case 0:
            return -width;
        
        //East
        case 1:
            return 1;
        
        //South
        case 2:
            return width;
        
        //West
        case 3:
            return -1;

        default:
            return 0;
    }
}

int validate_input(enum input_key input, int snake_dir, int snake_grow){

    //going down edge case
    if(snake_grow == 1){
    if(input == INPUT_UP && snake_dir != 2){
        return 0;
    }

    //going up edge case
    if(input == INPUT_DOWN && snake_dir != 0){
        return 2;
    }

    //going right edge case
    if(input == INPUT_LEFT && snake_dir != 1){
        return 3;
    }

    //going left edge case
    if(input == INPUT_RIGHT && snake_dir != 3){
        return 1;
    }
}

else{


    if(input == INPUT_UP){
        return 0;
    }

    if(input == INPUT_DOWN){
        return 2;
    }

    if(input == INPUT_LEFT){
        return 3;
    }

    if(input == INPUT_RIGHT){
        return 1;
    }

}
    

    return snake_dir;

}

bool eat_food(int* snake_head, int* cells, int new_index){

    //get value at index
    int value_at_index = *(cells + new_index);

    //is it food?
    if(value_at_index == FLAG_FOOD){
        return true;
    }

    return false;
}

void grow_snake(snake_t* snake_p, int* cells, int new_index){
    //get last node in snake as of now
    node_t* last_node = get_last(snake_p->snake_head);

    //add new node to snake
    node_t* new_node = malloc(sizeof(node_t));
    new_node->data = snake_p->snake_head->data - 1;
    //connect nodes
    last_node->next = new_node;
    new_node->prev = last_node;
    new_node->next = NULL;
    new_node->dir = last_node->dir;

}