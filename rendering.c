#include <raylib.h>

#include "game.h"
#include "rendering.h"

void RenderRunningState(const game_t *game);
void RenderGameOverState(const game_t *game);
void RenderPauseState(const game_t *game);

void RenderGame(const game_t *game){
    switch(game->state){
        case RUNNING_STATE:
            RenderRunningState(game);
            break;
        case PAUSE_STATE:
            RenderPauseState(game);
            break;
        case GAME_OVER_STATE:
            RenderGameOverState(game);
            break;
    }
}

void RenderRunningState(const game_t *game){
    //renders shadow piece
    for(int col = 0; col < 4; col++){
        for(int row = 0; row < 4; row++){
            if(game->tetrominos[game->piece_index][game->tet_rotation][row][col] == FALLING_SQUARE){
                DrawRectangle((col + game->piece_col)*CELL_WIDTH, (row + game->lowest_piece_row)*CELL_HEIGHT,
                              CELL_WIDTH, CELL_HEIGHT, LIGHTGRAY);
            }
        }
    }

    // renders the board
    for(int row = 0; row < ROWS; row++){
        for(int col = 0; col < COLUMNS; col++){
            if(game->play_area[row][col] != EMPTY){
                DrawRectangle(col*CELL_WIDTH, row*CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT, game->piece_color_index[game->play_area[row][col]]);
            }
        }
    }

    //renders the falling piece
    for(int col = 0; col < 4; col++){
        for(int row = 0; row < 4; row++){
            if(game->tetrominos[game->piece_index][game->tet_rotation][row][col] == FALLING_SQUARE){
                DrawRectangle((col + game->piece_col)*CELL_WIDTH, (row + game->piece_row)*CELL_HEIGHT,
                              CELL_WIDTH, CELL_HEIGHT, game->piece_color_index[game->piece_index]);
            }
        }
    }

    //render grid
#if DRAW_GRID
    for(int i = 1; i <= COLUMNS; i++){
        DrawLine(i * CELL_WIDTH, 0, i*CELL_WIDTH, GAME_SCREEN_HEIGHT, BLACK);
    }

    for(int i = 1; i <= ROWS; i++){
        DrawLine(0, i * CELL_HEIGHT, GAME_SCREEN_WIDTH, i*CELL_HEIGHT, BLACK);
    }
#else

DrawLine(COLUMNS * CELL_WIDTH, 0, COLUMNS*CELL_WIDTH, GAME_SCREEN_HEIGHT, BLACK);

#endif
    // render RenderMiniPieces
    if(game->alt_init == true){
        for(int col = 0; col < 4; col++){
            for(int row = 0; row < 4; row++){
                if(game->tetrominos[game->alt_index][0][row][col] == FALLING_SQUARE){
                    DrawRectangle((col*ALT_CELL)+GAME_SCREEN_WIDTH+123, (row*ALT_CELL)+ 10, ALT_CELL, ALT_CELL,
                            game->piece_color_index[game->alt_index]);
                }
            }
        }
    }

    for(int i = 0; i < 3; i++){
        for(int col = 0; col < 4; col++){
            for(int row = 0; row < 4; row++){
                if(game->tetrominos[game->cached_index[i]][0][row][col] == FALLING_SQUARE){
                    DrawRectangle((col*TINY_CELL)+GAME_SCREEN_WIDTH+148, ((row+i*3)*TINY_CELL)+ 110, TINY_CELL, TINY_CELL,
                            game->piece_color_index[game->cached_index[i]]);
                }
            }
        }
    }

    //render score and lines;
    DrawText(game->score_c, GAME_SCREEN_WIDTH+10, 780, 30, BLACK);
    DrawText(game->lines_c, GAME_SCREEN_WIDTH+10, 810, 30, BLACK);
}


void RenderPauseState(const game_t *game){
    DrawText("PAUSED", SCREEN_WIDTH/3-60, SCREEN_HEIGHT/3+20, 100, GRAY);
}
void RenderGameOverState(const game_t *game){
    DrawText("GAME OVER", SCREEN_WIDTH/3-160, SCREEN_HEIGHT/3+20, 100, BLACK);
    DrawText(game->score_c, SCREEN_WIDTH/2-100, SCREEN_HEIGHT/2+20, 40, BLACK);
    DrawText(game->lines_c, SCREEN_WIDTH/2-100, SCREEN_HEIGHT/2+60,40, BLACK);
}
