#include <raylib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "rendering.h"
#include "logic.h"

int main(){
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tetris");
    SetWindowPosition(960 - SCREEN_WIDTH/2, 520);
    //InitAudioDevice();
    SetExitKey(0);
    SetTargetFPS(60);

    SetRandomSeed(time(NULL));
    int r = GetRandomValue(0, 6);
    game_t game = {
        .tet_rotation = 0,
        .piece_index = r,
        .score = 0,
        .cached_index = {0},
        .alt_index = 0,
        .alt_init = false,
        .game_init = false,
        .line_cleared = 0,
        .piece_col = 3,
        .piece_row = 0,
        .lowest_piece_row = 0,
        .state = RUNNING_STATE,

        .score_c = "score: 0",
        .lines_c = "lines: 0",
        .piece_color_index = {BLUE, DARKBLUE, ORANGE, YELLOW, GREEN, PURPLE, RED},
        .tetrominos =
        {
            {
                {// I piece
                    {e,e,e,e},
                    {F,F,F,F},
                    {e,e,e,e},
                    {e,e,e,e},
                },
                {
                    {e,e,F,e},
                    {e,e,F,e},
                    {e,e,F,e},
                    {e,e,F,e},
                },
                {
                    {e,e,e,e},
                    {e,e,e,e},
                    {F,F,F,F},
                    {e,e,e,e},
                },
                {
                    {e,F,e,e},
                    {e,F,e,e},
                    {e,F,e,e},
                    {e,F,e,e},
                }
            },
            {// left L piece
                {
                    {F,e,e,e},
                    {F,F,F,e},
                    {e,e,e,e},
                    {e,e,e,e},
                },
                {
                    {e,F,F,e},
                    {e,F,e,e},
                    {e,F,e,e},
                    {e,e,e,e},
                },
                {
                    {e,e,e,e},
                    {F,F,F,e},
                    {e,e,F,e},
                    {e,e,e,e},
                },
                {
                    {e,F,e,e},
                    {e,F,e,e},
                    {F,F,e,e},
                    {e,e,e,e},
                }
            },
            {//Right L piece
                {
                    {e,e,F,e},
                    {F,F,F,e},
                    {e,e,e,e},
                    {e,e,e,e},
                },
                {
                    {e,F,e,e},
                    {e,F,e,e},
                    {e,F,F,e},
                    {e,e,e,e},
                },
                {
                    {e,e,e,e},
                    {F,F,F,e},
                    {F,e,e,e},
                    {e,e,e,e},
                },
                {
                    {F,F,e,e},
                    {e,F,e,e},
                    {e,F,e,e},
                    {e,e,e,e},
                }
            },
            {//Square piece
                {
                    {e,F,F,e},
                    {e,F,F,e},
                    {e,e,e,e},
                    {e,e,e,e},
                },
                {
                    {e,F,F,e},
                    {e,F,F,e},
                    {e,e,e,e},
                    {e,e,e,e},
                },
                {
                    {e,F,F,e},
                    {e,F,F,e},
                    {e,e,e,e},
                    {e,e,e,e},
                },
                {
                    {e,F,F,e},
                    {e,F,F,e},
                    {e,e,e,e},
                    {e,e,e,e},
                }
            },
            {//S piece
                {
                    {e,F,F,e},
                    {F,F,e,e},
                    {e,e,e,e},
                    {e,e,e,e},
                },
                {
                    {e,F,e,e},
                    {e,F,F,e},
                    {e,e,F,e},
                    {e,e,e,e},
                },
                {
                    {e,e,e,e},
                    {e,F,F,e},
                    {F,F,e,e},
                    {e,e,e,e},
                },
                {
                    {F,e,e,e},
                    {F,F,e,e},
                    {e,F,e,e},
                    {e,e,e,e},
                }
            },
            {//T piece
                {
                    {e,F,e,e},
                    {F,F,F,e},
                    {e,e,e,e},
                    {e,e,e,e},
                },
                {
                    {e,F,e,e},
                    {e,F,F,e},
                    {e,F,e,e},
                    {e,e,e,e},
                },
                {
                    {e,e,e,e},
                    {F,F,F,e},
                    {e,F,e,e},
                    {e,e,e,e},
                },
                {
                    {e,F,e,e},
                    {F,F,e,e},
                    {e,F,e,e},
                    {e,e,e,e},
                }
            },
            {//Z piece
                {
                    {F,F,e,e},
                    {e,F,F,e},
                    {e,e,e,e},
                    {e,e,e,e},
                },
                {
                    {e,e,F,e},
                    {e,F,F,e},
                    {e,F,e,e},
                    {e,e,e,e},
                },
                {
                    {e,e,e,e},
                    {F,F,e,e},
                    {e,F,F,e},
                    {e,e,e,e},
                },
                {
                    {e,F,e,e},
                    {F,F,e,e},
                    {F,e,e,e},
                    {e,e,e,e},
                }
            },
        },
    };

    memset(game.play_area, EMPTY, sizeof(game.play_area));

    while(!WindowShouldClose()){
        RunGame(&game);
        BeginDrawing();
            ClearBackground(RAYWHITE);
            RenderGame(&game);
        EndDrawing();
    }
    //StopMusicStream(music);

    //CloseAudioDevice();

    CloseWindow();

    return 0;
}
