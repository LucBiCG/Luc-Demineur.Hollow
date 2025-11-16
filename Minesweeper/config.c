#include "config.h"
#include "basics.h"
// Variables globales pour la configuration actuelle
static int currentGridSize = GRID_SIZE;
static int currentBombCount = BOMB_COUNT;
static float currentCellSize = CELL_SIZE;

void SetGameDifficulty(int difficulty)
{
    switch (difficulty) {
    case 0: // Facile
        currentGridSize = EASY_GRID_SIZE;
        currentBombCount = EASY_BOMB_COUNT;
        break;
    case 1: // Moyen
        currentGridSize = MEDIUM_GRID_SIZE;
        currentBombCount = MEDIUM_BOMB_COUNT;
        break;
    case 2: // Difficile
        currentGridSize = HARD_GRID_SIZE;
        currentBombCount = HARD_BOMB_COUNT;
        break;
    default: // Normal
        currentGridSize = GRID_SIZE;
        currentBombCount = BOMB_COUNT;
    }

    // Calculer la taille des cellules
    currentCellSize = (WIDTH - 2 * GRID_OFFSET) / (float)currentGridSize;

    printf("Configuration: Grille %dx%d, %d bombes, taille cellule: %.1f\n",
        currentGridSize, currentGridSize, currentBombCount, currentCellSize);
}

int GetGridSize() {
    return currentGridSize;
}

int GetBombCount() {
    return currentBombCount;
}

float GetCellSize() {
    return currentCellSize;
}