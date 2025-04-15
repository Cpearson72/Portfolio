#ifndef GAME_H
#define GAME_H

#include <stddef.h>
#include <stdbool.h>

#include "common.h"

void read_name(char* write_into);
void update(int* cells, size_t width, size_t height, snake_t* snake_p,
            enum input_key input, int growing);
void update_board_with_growth(int* cells, snake_t* snake_p, int move_amount, size_t width, size_t height, int curr_dir);
void place_food(int* cells, size_t width, size_t height);
int where_to_place_tail(node_t* tail);
void teardown(int* cells, snake_t* snake_p);

//use move amount to test if we drop out of the allocated memory for cells
bool memory_safe(int* snake_head, int* cells, size_t width, size_t height, int moveamount);

bool good_move(int* snake_head, int* cells, size_t width, size_t height, int moveamount);

int get_move_amount(int snake_dir, size_t width, size_t height);

int validate_input(enum input_key input, int snake_dir, int snake_growth);

bool eat_food(int* snake_head, int* cells, int new_index);

void grow_snake(snake_t* snake_p, int* cells, int new_index);


#endif
