#include <SFML/Audio.h>
#include <SFML/Graphics.h>

#include "basics.h"
#include "Grid.h"

#define DEBUG_CLEAN false

int main(void)
{
    // Define the video mode and create the window
    sfVideoMode mode = { WIDTH, HEIGHT, 32 };
    sfRenderWindow* window;
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
                if (event.mouseButton.button == sfMouseLeft)
                {
                    sfVector2i mousePos = { event.mouseButton.x, event.mouseButton.y };
                    sfVector2i cellPos = { mousePos.x / 30, mousePos.y / 30 };

                    if (cellPos.x >= 0 && cellPos.x < GRID_SIZE &&
                        cellPos.y >= 0 && cellPos.y < GRID_SIZE)
                    {
                        printf("Clic sur cellule (%d, %d)\n", cellPos.x, cellPos.y);

                        if (bFirstTouch) {
                            printf("Plantation des bombes...\n");
                            GridPlantBomb(grid, BOMB_COUNT, cellPos);
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
                }
                else if (event.mouseButton.button == sfMouseRight)
                {
                    sfVector2i mousePos = { event.mouseButton.x, event.mouseButton.y };
                    sfVector2i cellPos = { mousePos.x / 30, mousePos.y / 30 };

                    if (cellPos.x >= 0 && cellPos.x < GRID_SIZE &&
                        cellPos.y >= 0 && cellPos.y < GRID_SIZE)
                    {
                        printf("Drapeau sur cellule (%d, %d)\n", cellPos.x, cellPos.y);
                        CellFlag(grid, cellPos);
                    }
                }
            }
        }

        /* Clear the screen */
        sfRenderWindow_clear(window, sfBlack);

        // Draw the grid
        GridDraw(grid, window);

        sfText* debugCell = sfText_create();/* Temporaire */
        sfFont* debugFont = sfFont_createFromFile("C:/Windows/Fonts/tahoma.ttf");
        if (debugFont) {
            sfText_setFont(debugCell, debugFont);
            sfText_setString(debugCell, "");
            sfText_setCharacterSize(debugCell, 16);
            sfText_setColor(debugCell, sfWhite);
            sfText_setPosition(debugCell, (sfVector2f) { 200, 10 });
            sfRenderWindow_drawText(window, debugCell, NULL);
        }

        sfText_destroy(debugCell);
        if (debugFont) sfFont_destroy(debugFont);
        // DEBUG: Afficher les textes de test
        if (testFont1) {
            sfText_setPosition(testText1, (sfVector2f) { 10, 10 });
            sfText_setCharacterSize(testText1, 16);
            sfText_setColor(testText1, sfGreen);
            sfRenderWindow_drawText(window, testText1, NULL);
        }

        if (testFont2) {
            sfText_setPosition(testText2, (sfVector2f) { 10, 40 });
            sfText_setCharacterSize(testText2, 16);
            sfText_setColor(testText2, sfYellow);
            sfRenderWindow_drawText(window, testText2, NULL);
        }

        if (testFont3) {
            sfText_setPosition(testText3, (sfVector2f) { 10, 70 });
            sfText_setCharacterSize(testText3, 16);
            sfText_setColor(testText3, sfCyan);
            sfRenderWindow_drawText(window, testText3, NULL);
        }

        /* Update the window */
        sfRenderWindow_display(window);
    }

    /* Cleanup resources */
    // Libérer les ressources de test
    if (testText1) sfText_destroy(testText1);
    if (testFont1) sfFont_destroy(testFont1);
    if (testText2) sfText_destroy(testText2);
    if (testFont2) sfFont_destroy(testFont2);
    if (testText3) sfText_destroy(testText3);
    if (testFont3) sfFont_destroy(testFont3);

    GridDestroy(grid);
    sfRenderWindow_destroy(window);

    return SUCCESS;
}