#include <stdbool.h>
#include <raylib.h>
#include "raylib_or_curses.h"
//#include <ncurses.h> or what evs

#if USING_RAYLIB
//define functions as raylib
#else
//define functions for ncurses
#endif

// the functions need to use the same name to work as wrappers
