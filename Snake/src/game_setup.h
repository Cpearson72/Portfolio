#ifndef GAME_SETUP_H
#define GAME_SETUP_H

#include "common.h"
#include "game.h"

extern bool default_board;

/** Enum to communicate board initialization status.
 * Values include INIT_SUCCESS, INIT_ERR_INCORRECT_DIMENSIONS,
 * INIT_ERR_WRONG_SNAKE_NUM, and INIT_ERR_BAD_CHAR.
 */
enum board_init_status {
    INIT_SUCCESS,                   // no errors were thrown
    INIT_ERR_INCORRECT_DIMENSIONS,  // dimensions description was not formatted
                                    // correctly, or too many rows/columns are
                                    // specified anywhere in the string for the
                                    // given dimensions
    INIT_ERR_WRONG_SNAKE_NUM,  // no snake or multiple snakes are on the board
    INIT_ERR_BAD_CHAR,  // any other part of the compressed string was formatted
                        // incorrectly
    INIT_UNIMPLEMENTED  // only used in stencil, no need to handle this
};

enum board_init_status initialize_game(int** cells_p, size_t* width_p,
                                       size_t* height_p, snake_t* snake_p,
                                       char* board_rep);

enum board_init_status decompress_board_str(int** cells_p, size_t* width_p,
                                            size_t* height_p, snake_t* snake_p,
                                            char* compressed);
enum board_init_status initialize_default_board(int** cells_p, size_t* width_p,
                                                size_t* height_p, snake_t* snake_p);

bool handle_board_dimensions(char* dimensions, size_t* width_p,
                                               size_t* height_p);

bool handle_row_rep(int** cells_p, char* row, char** row_rep, size_t* width, size_t* height_p);

void grab_char(char* curr_char, char* row, long unsigned int* index, int* num);

bool is_good_char(char c);

enum board_init_status board_set_up(int** cells_p, size_t width, size_t height, char* board_rep, snake_t* snake_p);

#endif
