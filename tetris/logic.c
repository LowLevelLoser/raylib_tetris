#include <raylib.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "game.h"
#include "logic.h"

void MoveLeft(game_t *game);
void MoveRight(game_t *game);
void MoveDown(game_t *game);
void Spin(game_t *game);
void Drop(game_t *game);
void Pause(game_t *game);
void SwitchPiece(game_t *game);

bool Overlaps(game_t *game);
bool Collides(game_t *game);
int FindFullLine(game_t *game);
int GetLowestPossibleRow(game_t *game);
void SettlePiece(game_t *game);
void ResetGame(game_t *game);
void NextPiece(game_t *game);
void ResetPlacement(game_t *game);
void ClearFullLines(game_t *game);
bool EdgeCorrect(game_t *game);

bool settled = false;
bool canSwitch = true;
float fallTime = 0;
float timeToFall = TIME_TO_FALL;

void RunGame(game_t *game){
    static float left_time = KEY_DELAY;
    static float right_time = KEY_DELAY;
    static float down_time = KEY_DELAY;
    if(game->state == RUNNING_STATE){
        fallTime += GetFrameTime();
        if(game->game_init == false){
            for(int i = 0; i < 3; i++){
                SetRandomSeed(GetRandomValue(0, 20));
                game->cached_index[i] = GetRandomValue(0, 6);
            }
            game->game_init = true;
        }

        if(fallTime >= timeToFall){
            MoveDown(game);
        }

        if(IsKeyDown(KEY_LEFT)){
            if(left_time >= KEY_DELAY){
                MoveLeft(game);
                left_time = 0;
            }
        }
        else {
            left_time = KEY_DELAY;
        }
        if(IsKeyDown(KEY_RIGHT)){
            if(right_time >= KEY_DELAY){
                MoveRight(game);
                right_time = 0;
            }
        }
        else {
            right_time = KEY_DELAY;
        }
        if(IsKeyDown(KEY_DOWN)){
            if(down_time >= KEY_DELAY){
                MoveDown(game);
                down_time = 0;
            }
        }
        else {
            down_time = KEY_DELAY;
        }

        switch(GetKeyPressed()){
             case KEY_UP:
                Spin(game);
                break;
            case KEY_SPACE:
                Drop(game);
                break;
            case KEY_C:
                SwitchPiece(game);
                break;
            case KEY_R:
                ResetGame(game);
                break;
            case KEY_ESCAPE:
                game->state = PAUSE_STATE;
                break;
        }
        left_time += GetFrameTime();
        right_time += GetFrameTime();
        down_time += GetFrameTime();

        game->lowest_piece_row = GetLowestPossibleRow(game);
    }

    else{
        switch(GetKeyPressed()){
            case KEY_R:
                ResetGame(game);
                break;
            case KEY_ESCAPE:
                game->state = RUNNING_STATE;
                break;
        }
    }
}

int GetLowestPossibleRow(game_t *game){
    int row_c = game->piece_row;
    int lowest_row = row_c;
    while(Collides(game) == false){
        game->piece_row++;
    }
    lowest_row = game->piece_row - 1;
    game->piece_row = row_c;
    return lowest_row;
}

void MoveLeft(game_t *game){
    game->piece_col--;
    if(Collides(game) == true){
        game->piece_col++;
    }
}

void MoveRight(game_t *game){
    game->piece_col++;
    if(Collides(game) == true){
        game->piece_col--;
    }
    game->piece_row++;
    if(Collides(game) == true){
        fallTime -= KEY_DELAY - .01;
    }
    game->piece_row--;
}

void MoveDown(game_t *game){
    fallTime = 0;
    game->piece_row++;
    if(Collides(game) == true){
        game->piece_row--;
        SettlePiece(game);
    }
    game->piece_row++;
    if(Collides(game) == true){
        fallTime -= KEY_DELAY - .01;
    }
    game->piece_row--;
}

void Spin(game_t *game) {
    if(game->piece_row == -1){
        game->piece_row++;
    }
    int b = game->tet_rotation;
    game->tet_rotation = (game->tet_rotation+1)%4;
    if(Collides(game) == true){
        if (EdgeCorrect(game) == true){
            game->tet_rotation = b;
        }
    }
    game->piece_row++;
    if(Collides(game) == true){
        fallTime -= 0.1;
    }
    game->piece_row--;
}


void Drop(game_t *game){
    settled = false;
    while(settled == false){
        MoveDown(game);
    }
    game->score += 10;
    snprintf(game->score_c, 50, "score: %d", game->score);
}

void SwitchPiece(game_t *game){
    if (canSwitch == true){
        enum piece_t b = game->piece_index;

        if(game->alt_init == true){
            game->piece_index = game->alt_index;
            game->alt_index = b;
        }
        else{
            game->alt_index = b;
            game->alt_init = true;
            game->piece_index = game->cached_index[0];
            NextPiece(game);
        }
        ResetPlacement(game);
        canSwitch = false;
    }
}

bool Collides(game_t *game){
    for(int col = 0; col < 4; col++){
        for(int row = 0; row < 4; row++){
            if(game->tetrominos[game->piece_index][game->tet_rotation][col][row] == FALLING_SQUARE){
                if(game->piece_col+row >= COLUMNS || game->piece_col+row < 0){
                    return true;
                }
                else if(game->piece_row+col >= ROWS){
                    return true;
                }
            }
        }
    }
    return Overlaps(game);
}

bool EdgeCorrect(game_t *game){
    for(int i = 0; i < 2; i++){
        if(Collides(game) == true){
            game->piece_col--;
        }
    }
    if(Collides(game) == true){
        game->piece_col = game->piece_col + 2;
    }

    for(int i = 0; i < 2; i++){
        if(Collides(game) == true){
            game->piece_col++;
        }
    }
    if(Collides(game) == true){
        game->piece_col = game->piece_col - 2;
    }

    for(int i = 0; i < 2; i++){
        if(Collides(game) == true){
            game->piece_row--;
        }
    }
    if(Collides(game) == true){
        game->piece_row = game->piece_row + 2;
    }
    //reset_overlaps();
    return Collides(game);
}

bool Overlaps(game_t *game){
    for(int col = 0; col < 4; col++){
        for(int row = 0; row < 4; row++){
            if(game->tetrominos[game->piece_index][game->tet_rotation][row][col] == FALLING_SQUARE
               && game->play_area[game->piece_row+row][game->piece_col+col] !=  EMPTY){
                return true;
            }
        }
    }
    return false;
}

void SettlePiece(game_t *game) {
    if(game->piece_row <= 0){
        game->state = GAME_OVER_STATE;
    }
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            if (game->tetrominos[game->piece_index][game->tet_rotation][row][col] == FALLING_SQUARE)
                game->play_area[game->piece_row + row][game->piece_col + col] = game->piece_index;
        }
    }
    settled = true;
    NextPiece(game);
    ResetPlacement(game);
    ClearFullLines(game);
    canSwitch = true;
}

void ClearFullLines(game_t *game){
    static int temp_lines = 0;
    int line = FindFullLine(game);
    if(line == -1){
        game->line_cleared += temp_lines;
        game->score += 100*temp_lines*temp_lines;
        temp_lines = 0;
        snprintf(game->score_c, 50, "score: %d", game->score);
        snprintf(game->lines_c, 50, "lines: %d", game->line_cleared);
        return;
    }

    for (int col = 0; col < COLUMNS; col++){
        game->play_area[0][col] = EMPTY;
    }

    if(line > 0){
        for(int col = 0; col < COLUMNS; col++)
            for(int row = line; row > 0; row--){
                game->play_area[row][col] = game->play_area[row-1][col];
            }
        temp_lines++;
    }

    ClearFullLines(game);
    if (timeToFall > MIN_TIME_TO_FALL){
        timeToFall = TIME_TO_FALL - game->line_cleared / 300.0;
    }
}

int FindFullLine(game_t *game){
    int total = 0;
    for(int row = 0; row < ROWS; row++){
        for(int col = 0; col < COLUMNS; col++){
            total += (game->play_area[row][col] != EMPTY);
        }
        if(total == CLEAR_BAR){
            return row;
        }
        total = 0;
    }
    total = 0;
    return -1;
}

void NextPiece(game_t *game){
    SetRandomSeed(time(NULL));
    int r = GetRandomValue(0, 6);
    game->piece_index = game->cached_index[0];
    if(r == game->cached_index[2]){
        r =(r+1)%7;
    }
    for(int i = 0; i < 3; i++){
        if(i == 2)
            game->cached_index[i] = r;
        else{
            game->cached_index[i] = game->cached_index[i+1];
        }
    }
}

void ResetPlacement(game_t *game){
    switch(game->piece_index){
        case I_PIECE:
            game->piece_row = -1;
            break;
        default:
            game->piece_row = 0;
    }
    game->tet_rotation = 0;
    game->piece_col = 3;
}

void ResetGame(game_t *game){
    memset(game->play_area, EMPTY, sizeof(int)*ROWS*COLUMNS);
    game->state = RUNNING_STATE;
    game->score = 0;
    game->line_cleared = 0;
    strcpy(game->score_c, "score: 0");
    strcpy(game->lines_c, "lines: 0");
    game->alt_init = false;
    canSwitch = true;
    timeToFall = TIME_TO_FALL;
    ResetPlacement(game);
}
