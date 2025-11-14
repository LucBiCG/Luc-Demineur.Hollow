#include <SFML/Graphics.h>

#include "Grid.h"
static sfText* CreateText(const char* text, const char* fontFilePath, const int size)
{
	// Use sfText_create() to create a Text and contains it in a sfText pointer
	sfText* newText = sfText_create();
	// Create its font with sfFont_createFromFile(fontFilePath) contains it in a sfFont pointer, 
	// where fontFilePath is the path to find the .ttf file in your project
	sfFont* newFont = sfFont_createFromFile(fontFilePath);
	if (!newFont) { // Check if it found the font file to be sure
		printf("Erreur: Impossible de charger la police depuis %s\n", fontFilePath);
		return NULL;
	}

	sfText_setFont(newText, newFont);
	sfText_setString(newText, text);
	sfText_setCharacterSize(newText, size);
	sfText_setColor(newText, sfBlack);

	return newText;
}
static sfFont* globalFont = NULL;
Cell* CellCreate(sfVector2f size, sfVector2f pos, sfColor color) 
{
	// Initialize all cell properties
	// ...
	
	Cell* newCell = (Cell*)malloc(sizeof(Cell));
	if (newCell == NULL) {
		return NULL;
	}

	// Initialisation de la forme
	newCell->shape = sfRectangleShape_create();
	sfRectangleShape_setSize(newCell->shape, size);
	sfRectangleShape_setPosition(newCell->shape, pos);
	sfRectangleShape_setFillColor(newCell->shape, color);
	sfRectangleShape_setOutlineColor(newCell->shape, sfBlack);
	sfRectangleShape_setOutlineThickness(newCell->shape, 1.0f);

	// Initialisation du texte avec tahoma.ttf
	newCell->text = sfText_create();
	sfFont* cellFont = sfFont_createFromFile("C:/Windows/Fonts/tahoma.ttf");

	if (cellFont) {
		sfText_setFont(newCell->text, cellFont);
		printf("SUCCES: Police tahoma chargee pour cellule\n");
	}
	else {
		printf("ECHEC: Police tahoma non chargee\n");
	}

	sfText_setString(newCell->text, "");
	sfText_setCharacterSize(newCell->text, 20);
	sfText_setColor(newCell->text, sfBlack);

	// MEILLEUR POSITIONNEMENT du texte
	sfVector2f textPos = {
		pos.x + (size.x / 2) - 5,  // Centrage horizontal
		pos.y + (size.y / 2) - 10  // Centrage vertical
	};
	sfText_setPosition(newCell->text, textPos);

	// Initialisation des propriétés
	newCell->bDiscovered = false;
	newCell->bFlagged = false;
	newCell->bPlanted = false;
	newCell->explosiveNeighbor = 0;

	return newCell;
}
void CellDraw(Cell* cell, sfRenderWindow* window)
{
	// Draw the cell shape and text on the window
	// ...
	sfRenderWindow_drawRectangleShape(window, cell->shape, NULL);

	// Ensuite dessiner le texte PAR-DESSUS le rectangle
	if (cell->bDiscovered) {
		if (cell->bPlanted) {
			// Bombe - texte "X"
			sfText_setString(cell->text, "X");
			sfText_setColor(cell->text, sfRed);
			sfRenderWindow_drawText(window, cell->text, NULL);
		}
		else if (cell->explosiveNeighbor > 0) {
			// Numéro
			char numberStr[2];
			snprintf(numberStr, sizeof(numberStr), "%d", cell->explosiveNeighbor);
			sfText_setString(cell->text, numberStr);

			// Couleurs selon le numéro
			switch (cell->explosiveNeighbor) {
			case 1: sfText_setColor(cell->text, sfBlue); break;
			case 2: sfText_setColor(cell->text, sfGreen); break;
			case 3: sfText_setColor(cell->text, sfRed); break;
			case 4: sfText_setColor(cell->text, sfColor_fromRGB(0, 0, 128)); break;
			case 5: sfText_setColor(cell->text, sfColor_fromRGB(128, 0, 0)); break;
			case 6: sfText_setColor(cell->text, sfColor_fromRGB(0, 128, 128)); break;
			case 7: sfText_setColor(cell->text, sfBlack); break;
			case 8: sfText_setColor(cell->text, sfColor_fromRGB(64, 64, 64)); break;
			}
			sfRenderWindow_drawText(window, cell->text, NULL);
		}
	}
	else if (cell->bFlagged) {
		// Drapeau
		sfText_setString(cell->text, "F");
		sfText_setColor(cell->text, sfRed);
		sfRenderWindow_drawText(window, cell->text, NULL);
	}
}

int CellReveal(Grid* grid, sfVector2i cellGridPos)
{
	Cell* cell = grid->cells[cellGridPos.x][cellGridPos.y];

	if (cell->bDiscovered || cell->bFlagged) {
		return 0;
	}

	if (cell->bPlanted) {
		printf("CELLULE (%d,%d): BOMBE!\n", cellGridPos.x, cellGridPos.y);
		return FAILURE;
	}

	// Révéler la cellule
	sfRectangleShape_setFillColor(cell->shape, sfColor_fromRGB(220, 220, 220));
	cell->bDiscovered = true;

	printf("CELLULE (%d,%d): decouverte, bombes voisines = %d\n",
		cellGridPos.x, cellGridPos.y, cell->explosiveNeighbor);

	if (cell->explosiveNeighbor > 0) {
		char numberStr[2];
		snprintf(numberStr, sizeof(numberStr), "%d", cell->explosiveNeighbor);
		sfText_setString(cell->text, numberStr);
		printf("TEXTE MIS A JOUR: %s\n", numberStr);

		// Couleurs...
		switch (cell->explosiveNeighbor) {
		case 1: sfText_setColor(cell->text, sfBlue); break;
		case 2: sfText_setColor(cell->text, sfGreen); break;
		case 3: sfText_setColor(cell->text, sfRed); break;
			// ... autres couleurs
		}
	}
	else {
		sfText_setString(cell->text, "");
	}

	// Flood fill pour les cases vides...
	if (cell->explosiveNeighbor == 0) {
		for (int dx = -1; dx <= 1; dx++) {
			for (int dy = -1; dy <= 1; dy++) {
				int newX = cellGridPos.x + dx;
				int newY = cellGridPos.y + dy;

				if (newX >= 0 && newX < GRID_SIZE && newY >= 0 && newY < GRID_SIZE) {
					if (!(dx == 0 && dy == 0)) {
						Cell* neighbor = grid->cells[newX][newY];
						if (!neighbor->bDiscovered && !neighbor->bFlagged) {
							CellReveal(grid, (sfVector2i) { newX, newY });
						}
					}
				}
			}
		}
	}

	grid->discoveredCellCount++;

	// Vérification victoire...
	int totalCells = GRID_SIZE * GRID_SIZE;
	int bombCount = 0;
	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = 0; j < GRID_SIZE; j++) {
			if (grid->cells[i][j]->bPlanted) bombCount++;
		}
	}

	if (grid->discoveredCellCount >= (totalCells - bombCount)) {
		return SUCCESS;
	}

	return 0;
}
	// If the cell is already discovered or flagged, do nothing and return 0
	// ...
	
	// If the cell is planted with a bomb, return FAILURE to indicate game over
	// ...
	
	
	// Change the cell's appearance to revealed (lighter color) and mark it as discovered
	// ...

	// If the cell has explosive neighbors, display the number
	// ...

	// If the cell is completely empty (explosiveNeighbor == 0), start the "flood fill" (reveal neighbors) algorithm
	// ...
	

	// Increase grid discovered cell count and If all none planted cells are discovered, terminate the game (return SUCCESS)
	// ...

	// Return 0 as the cell was revealed and was not a bomb
	


void CellFlag(Grid* grid, sfVector2i cellGridPos)
{
	Cell* cell = grid->cells[cellGridPos.x][cellGridPos.y];

	// If the cell is already discovered, do nothing and return
	// ...

	// Toggle the flagged state of the cell and update its appearance accordingly
	// ...
	
	if (cell->bDiscovered) return;

	cell->bFlagged = !cell->bFlagged;

	if (cell->bFlagged) {
		// Couleur rouge clair bien visible
		sfRectangleShape_setFillColor(cell->shape, sfColor_fromRGB(255, 100, 100));
	}
	else {
		// Retour à la couleur normale non découverte
		sfRectangleShape_setFillColor(cell->shape, sfColor_fromRGB(120, 120, 120));
	}
}


void CellDestroy(Cell* cell)
{
	// Free all resources associated with the cell
	// ...
	
	if (cell->shape != NULL) {
		sfRectangleShape_destroy(cell->shape);
		cell->shape = NULL;
	}

	// Destroy the SFML text
	if (cell->text != NULL) {
		sfText_destroy(cell->text);
		cell->text = NULL;
	}

	// Free the cell structure itself
	free(cell);
}


Grid* GridCreate()
{
	// Initialize grid properties, create all cell and register them in grid array
	// ...
	
	

	Grid* newGrid = (Grid*)malloc(sizeof(Grid));
	if (!newGrid) return NULL;

	newGrid->discoveredCellCount = 0;

	for (int x = 0; x < GRID_SIZE; x++) {
		for (int y = 0; y < GRID_SIZE; y++) {
			sfVector2f cellSize = { 30.0f, 30.0f };
			sfVector2f cellPos = { x * 30.0f, y * 30.0f };
			sfColor cellColor = sfColor_fromRGB(120, 120, 120); // Plus foncé

			newGrid->cells[x][y] = CellCreate(cellSize, cellPos, cellColor);

			// ASSOCIER LA POLICE AU TEXTE
			if (globalFont != NULL) {
				sfText_setFont(newGrid->cells[x][y]->text, globalFont);
			}
		}
	}

	return newGrid;
	}

void GridPlantBomb(Grid* grid, int bombCount, sfVector2i cellToAvoid)
{
	// Plant all bomb and avoid avoided spot
	// Update explosiveNeighbor count for all cells around each bomb planted
	// ...
	int bombsPlanted = 0;
	int attempts = 0;
	int maxAttempts = GRID_SIZE * GRID_SIZE * 2; // Safety limit

	while (bombsPlanted < bombCount && attempts < maxAttempts) {
		int randomX = rand() % GRID_SIZE;
		int randomY = rand() % GRID_SIZE;

		if ((randomX != cellToAvoid.x || randomY != cellToAvoid.y) &&
			!grid->cells[randomX][randomY]->bPlanted) {

			grid->cells[randomX][randomY]->bPlanted = true;
			bombsPlanted++;

			// Update neighbors
			for (int dx = -1; dx <= 1; dx++) {
				for (int dy = -1; dy <= 1; dy++) {
					int neighborX = randomX + dx;
					int neighborY = randomY + dy;

					if (neighborX >= 0 && neighborX < GRID_SIZE &&
						neighborY >= 0 && neighborY < GRID_SIZE) {
						grid->cells[neighborX][neighborY]->explosiveNeighbor++;
					}
				}
			}
		}
		attempts++;
	}

	// If we couldn't plant all bombs (shouldn't happen with reasonable bombCount)
	if (bombsPlanted < bombCount) {
		printf("Warning: Only planted %d out of %d bombs\n", bombsPlanted, bombCount);
	}
}


sfVector2i GridUpdateLoop(Grid* grid, sfRenderWindow* window) 
{
	sfVector2i mousePos = sfMouse_getPositionRenderWindow(window);

	// Convertir en coordonnées de cellule
	sfVector2i cellCoord = {
		mousePos.x / 30,
		mousePos.y / 30
	};

	// Vérifier si les coordonnées sont dans la grille
	if (cellCoord.x >= 0 && cellCoord.x < GRID_SIZE &&
		cellCoord.y >= 0 && cellCoord.y < GRID_SIZE) {
		return cellCoord;
	}

	// Retourner (-1, -1) si hors de la grille
	return (sfVector2i) { -1, -1 };
}


void GridDraw(Grid* grid, sfRenderWindow* window)
{
	// Draw all cells in the grid
	// ...
	{
		// Loop through all cells of the grid and call CellDraw to display them
		for (int x = 0; x < GRID_SIZE; x++) {
			for (int y = 0; y < GRID_SIZE; y++) {
				CellDraw(grid->cells[x][y], window);
			}
		}
	}
}

void GridDestroy(Grid* grid)
{
	// Free all resources associated with the grid and its cells
	// ...
	for (int x = 0; x < GRID_SIZE; x++) {
		for (int y = 0; y < GRID_SIZE; y++) {
			if (grid->cells[x][y] != NULL) {
				CellDestroy(grid->cells[x][y]);
				grid->cells[x][y] = NULL;
			}
		}
	}
	free(grid);

	// Libérer la police quand la dernière grille est détruite
	if (globalFont != NULL) {
		sfFont_destroy(globalFont);
		globalFont = NULL;
	}
}
