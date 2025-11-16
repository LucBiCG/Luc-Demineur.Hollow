#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

/// <summary>
/// Use this enum to define some parameters of the window
/// </summary>
enum WindowParam {
    WIDTH = 800,
    HEIGHT = 800,
};

/// <summary>
/// Use this enum to define some parameters of the Minesweeper grid
/// </summary>
enum MinesweeperParam {
    GRID_SIZE = 10,
    GRID_OFFSET = 50,
    CELL_SIZE = 66,
    CELL_OFFSET = 4,
    BOMB_COUNT = 15,
};

#define EASY_GRID_SIZE 8
#define EASY_BOMB_COUNT 10

#define MEDIUM_GRID_SIZE 12  
#define MEDIUM_BOMB_COUNT 20

#define HARD_GRID_SIZE 16
#define HARD_BOMB_COUNT 40

/// <summary>
/// Use this enum to define some exit codes for the program (just for debug purpose)
/// </summary>
enum ExitCode {
    FAILURE = -1,
    SUCCESS = 1,
    NULL_TEXT = 60,
    NULL_WINDOW = 61,
    NULL_SPRITE = 62,
    NULL_TEXTURE = 63,
    NULL_FONT = 64,
};
void SetGameDifficulty(int difficulty);