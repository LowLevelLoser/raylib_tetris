# TODO add build sccript for ncurses
cc -o Tetris main.c rendering.c logic.c -s -Wall -lraylib -lm -lpthread -ldl -lrt -lX11
