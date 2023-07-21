#ifndef GAME_H
#define GAME_H

#include <math.h>
#include <raylib.h>
#include <stdbool.h>

#define SCREEN_WIDTH 860
#define SCREEN_HEIGHT 860

#define KEY_DELAY .072
#define ROWS 20
#define COLUMNS 10
#define GAME_SCREEN_WIDTH (SCREEN_WIDTH/2)
#define GAME_SCREEN_HEIGHT SCREEN_HEIGHT
#define CELL_WIDTH (GAME_SCREEN_WIDTH/COLUMNS)
#define CELL_HEIGHT (GAME_SCREEN_HEIGHT/ROWS)
#define TINY_CELL 30
#define ALT_CELL CELL_WIDTH
#define TIME_TO_FALL 0.5

#define RUNNING_STATE 0
#define GAME_OVER_STATE 1
#define PAUSE_STATE 2

#define EMPTY -1
#define FALLING_SQUARE 1

#define e EMPTY
#define F FALLING_SQUARE

#define CLEAR_BAR COLUMNS

enum piece_t{
    I_PIECE,
    LEFT_L_PIECE,
    RIGHT_L_PIECE,
    SQUARE_PIECE,
    S_PIECE,
    T_PIECE,
    Z_PIECE,
};

typedef struct {
    int play_area[ROWS][COLUMNS];
    const int tetrominos[7][4][4][4];
    const Color piece_color_index[7];
    enum piece_t cached_index[3];
    enum piece_t piece_index;
    int piece_col;
    int piece_row;
    int lowest_piece_row;
    int tet_rotation;
    enum piece_t alt_index;

    int score;
    int line_cleared;
    int state;

    char score_c[50];
    char lines_c[50];

    bool alt_init;
    bool game_init;
} game_t;

#endif
