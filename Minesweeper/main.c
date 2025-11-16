#include <SFML/Audio.h>
#include <SFML/Graphics.h>

#include "basics.h"
#include "Grid.h"
#include "config.h"


#define DEBUG_CLEAN false

int main(void)
{
    
    printf("Choisissez la difficulte:\n");
    printf("0 - Facile (%dx%d, %d bombes)\n", EASY_GRID_SIZE, EASY_GRID_SIZE, EASY_BOMB_COUNT);
    printf("1 - Moyen (%dx%d, %d bombes)\n", MEDIUM_GRID_SIZE, MEDIUM_GRID_SIZE, MEDIUM_BOMB_COUNT);
    printf("2 - Difficile (%dx%d, %d bombes)\n", HARD_GRID_SIZE, HARD_GRID_SIZE, HARD_BOMB_COUNT);

    int difficulty;
    scanf_s("%d", &difficulty);
    SetGameDifficulty(difficulty);
     

    // Define the video mode and create the window
    sfVideoMode mode = { WIDTH, HEIGHT, 32 };
    sfRenderWindow* window = sfRenderWindow_create(mode, "Minesweeper", sfClose, NULL);
    sfEvent event;

    /* Create the main window */
    window = sfRenderWindow_create(mode, "Minesweeper", sfClose, NULL);
    if (!window)
    {
        return NULL_WINDOW;
    }

    // Seed the random number generator
    srand((unsigned int)time(NULL));

    // DEBUG: Test de police avant de créer la grille
    printf("=== TEST DE POLICE ===\n");

    // Test 1: Police avec chemin relatif
    sfText* testText1 = sfText_create();
    sfFont* testFont1 = sfFont_createFromFile("arial.ttf");
    if (testFont1) {
        printf("SUCCES: arial.ttf (chemin relatif)\n");
        sfText_setFont(testText1, testFont1);
        sfText_setString(testText1, "");// ajouter Test reussi - arial.ttf pour le debug
    }
    else {
        printf("ECHEC: arial.ttf (chemin relatif)\n");
    }

    // Test 2: Police avec chemin absolu Windows
    sfText* testText2 = sfText_create();
    sfFont* testFont2 = sfFont_createFromFile("C:/Windows/Fonts/arial.ttf");
    if (testFont2) {
        printf("SUCCES: C:/Windows/Fonts/arial.ttf\n");
        sfText_setFont(testText2, testFont2);
        sfText_setString(testText2, "");// Test reussi - chemin absolu pour le debug
    }
    else {
        printf("ECHEC: C:/Windows/Fonts/arial.ttf\n");
    }

    // Test 3: Autre police Windows
    sfText* testText3 = sfText_create();
    sfFont* testFont3 = sfFont_createFromFile("C:/Windows/Fonts/tahoma.ttf");
    if (testFont3) {
        printf("SUCCES: C:/Windows/Fonts/tahoma.ttf\n");
        sfText_setFont(testText3, testFont3);
        sfText_setString(testText3, "");// ajouter Test reussi - tahoma.ttf pour le debug
    }
    else {
        printf("ECHEC: C:/Windows/Fonts/tahoma.ttf\n");
    }

    // Initialize the game grid
    printf("=== CREATION DE LA GRILLE ===\n");
    Grid* grid = GridCreate();
    if (!grid) {
        printf("ECHEC: Creation de la grille\n");
        sfRenderWindow_destroy(window);
        return EXIT_FAILURE;
    }
    printf("SUCCES: Grille creee\n");

    printf("Start Game ! \n");
    bool bFirstTouch = true;
    int gameState = 0;

    /* Start the game loop */
    while (sfRenderWindow_isOpen(window))
    {
        /* Process events */
        while (sfRenderWindow_pollEvent(window, &event))
        {
            if (event.type == sfEvtClosed)
            {
                sfRenderWindow_close(window);
            }

            if (event.type == sfEvtMouseButtonPressed && gameState == 0)
            {
                // CORRECTION ICI : Utiliser GetCellSize() et GetGridSize()
                sfVector2i mousePos = { event.mouseButton.x, event.mouseButton.y };
                float cellSize = GetCellSize();
                int gridSize = GetGridSize();

                sfVector2i cellPos = {
                    (int)((mousePos.x - GRID_OFFSET) / cellSize),
                    (int)((mousePos.y - GRID_OFFSET) / cellSize)
                };

                printf("Clic en position: (%d, %d) -> Cellule: (%d, %d)\n",
                    mousePos.x, mousePos.y, cellPos.x, cellPos.y);

                if (cellPos.x >= 0 && cellPos.x < gridSize &&
                    cellPos.y >= 0 && cellPos.y < gridSize)
                {
                    if (event.mouseButton.button == sfMouseLeft) {
                        printf("Clic GAUCHE sur cellule (%d, %d)\n", cellPos.x, cellPos.y);

                        if (bFirstTouch) {
                            printf("Plantation des %d bombes...\n", GetBombCount());
                            GridPlantBomb(grid, GetBombCount(), cellPos);
                            bFirstTouch = false;
                        }

                        int result = CellReveal(grid, cellPos);
                        if (result == FAILURE) {
                            printf("=== GAME OVER ===\n");
                            gameState = FAILURE;
                        }
                        else if (result == SUCCESS) {
                            printf("=== VICTOIRE ===\n");
                            gameState = SUCCESS;
                        }
                    }
                    else if (event.mouseButton.button == sfMouseRight) {
                        printf("Clic DROIT sur cellule (%d, %d)\n", cellPos.x, cellPos.y);
                        CellFlag(grid, cellPos);
                    }
                }
                else {
                    printf("Clic HORS GRILLE\n");
                }
            }
        }

        /* Clear the screen */
        sfRenderWindow_clear(window, sfBlack);

        // Draw the grid
        GridDraw(grid, window);

        /* Update the window */
        sfRenderWindow_display(window);
    }

    /* Cleanup resources */
    GridDestroy(grid);
    sfRenderWindow_destroy(window);

    return SUCCESS;
}