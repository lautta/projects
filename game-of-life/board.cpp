#include <iostream>
#include "board.h"

using std::cout;
using std::endl;

/****************************************************************************************
**Function: placeBlinker
**Description: Places the oscillator/blinker pattern using user coordinates as center.
**Parameters: Oversized game world and row and column coords supplied by user input.
**Pre-conditions: gameWorld array has been initialized with false values and user has
**supplied validated row and column coords for center of pattern.
**Post-conditions: The blinker pattern has been placed at the desired location in
**the array.
****************************************************************************************/
void placeBlinker(bool gameWorld[60][80], int row, int column)
{
	gameWorld[row][column] = true;
	gameWorld[row][column + 1] = true;
 	gameWorld[row][column - 1] = true;
}

/****************************************************************************************
**Function: placeGlider
**Description: Places the glider pattern in world using user coordinates as center.
**Parameters: Oversized game world and row and column coords supplied by user input.
**Pre-conditions: gameWorld array has been initialized with false values and user has
**supplied validated row and column coords for center of pattern.
**Post-conditions: The glider pattern has been placed at the desired location in
**the array.
****************************************************************************************/
void placeGlider(bool gameWorld[60][80], int row, int column)
{
    gameWorld[row - 1][column - 1] = true;
	gameWorld[row - 1][column] = true;
 	gameWorld[row - 1][column + 1] = true;
	gameWorld[row][column - 1] = true;
	gameWorld[row + 1][column] = true;
}

/****************************************************************************************
**Function: placeGun
**Description: Places the gosper gun pattern in world using user coordinates as center.
**Parameters: Oversized game world and row and column coords supplied by user input.
**Pre-conditions: gameWorld array has been initialized with false values and user has
**supplied validated row and column coords for center of pattern.
**Post-conditions: The gun pattern has been placed at the desired location in
**the array.
****************************************************************************************/
void placeGun(bool gameWorld[60][80], int row, int column)
{
    gameWorld[row -4][column +6] = true;
    gameWorld[row -3][column +6] = true;
    gameWorld[row -3][column +4] = true;
    gameWorld[row -2][column -6] = true;
    gameWorld[row -2][column -5] = true;
    gameWorld[row -2][column +2] = true;
    gameWorld[row -2][column +3] = true;
    gameWorld[row -2][column +16] = true;
    gameWorld[row -2][column +17] = true;
    gameWorld[row -1][column -7] = true;
    gameWorld[row -1][column -3] = true;
    gameWorld[row -1][column +2] = true;
    gameWorld[row -1][column +3] = true;
    gameWorld[row -1][column +16] = true;
    gameWorld[row -1][column +17] = true;
    gameWorld[row][column -17] = true;
    gameWorld[row][column -18] = true;
    gameWorld[row][column -8] = true;
    gameWorld[row][column -2] = true;
    gameWorld[row][column +2] = true;
    gameWorld[row][column +3] = true;
    gameWorld[row +1][column -17] = true;
    gameWorld[row +1][column -18] = true;
    gameWorld[row +1][column -8] = true;
    gameWorld[row +1][column -4] = true;
    gameWorld[row +1][column -2] = true;
    gameWorld[row +1][column -1] = true;
    gameWorld[row +1][column +4] = true;
    gameWorld[row +1][column +6] = true;
    gameWorld[row +2][column -8] = true;
    gameWorld[row +2][column -2] = true;
    gameWorld[row +2][column +6] = true;
    gameWorld[row +3][column -7] = true;
    gameWorld[row +3][column -3] = true;
    gameWorld[row +4][column -6] = true;
    gameWorld[row +4][column -5] = true;
}

/****************************************************************************************
**Function: copyWorld
**Description: Copies game world array data to second array to store changes.
**Parameters: Two bool arrays that hold game world data.
**Pre-conditions: Array has been initialized or contains pattern and game world data.
**Post-conditions: The contents of array 1 have been copied to array 2.
****************************************************************************************/
void copyWorld(bool copyFrom[60][80], bool copyTo[60][80])
{
	for (int i = 0; i < 60; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			copyTo[i][j] = copyFrom[i][j];
		}
	}
}

/****************************************************************************************
**Function: simulateWorld
**Description: Checks neighbors of every cell of current game world, runs game logic for
**life and death, updates data into the temp array and then copies temp array to the
**game world for the next generation.
**Parameters: Oversized game world with "true" and "false" cells.
**Pre-conditions: gameWorld array contains game data of "true" and "false" cells from
**either initial pattern placement or previous generation simulation.
**Post-conditions: The game world array has been updated with the data for the next
**generation.
****************************************************************************************/
void simulateWorld(bool gameWorld[60][80])
{
    bool tempWorld[60][80];

    //copy gameWorld to temporary world
    copyWorld(gameWorld, tempWorld);

    /*loop to check neighbors of every cell of current game world and update
    status in temp game world. Leave a 1 cell buffer zone so the non-existent
    cells on the edges aren't checked*/
    for (int i = 1; i < 59; i++)
    {
        for (int j = 1; j < 79; j++)
        {
            //track the number of neighbors
            int neighbors = 0;

            //check the eight possible neighbor coords and add to total
            if (gameWorld[i - 1][j - 1])
            {
                neighbors++;
            }
            if (gameWorld[i - 1][j])
            {
                neighbors++;
            }
            if (gameWorld[i - 1][j + 1])
            {
                neighbors++;
            }
            if (gameWorld[i][j - 1])
            {
                neighbors++;
            }
            if (gameWorld[i][j + 1])
            {
                neighbors++;
            }
            if (gameWorld[i + 1][j - 1])
            {
                neighbors++;
            }
            if (gameWorld[i + 1][j])
            {
                neighbors++;
            }
            if (gameWorld[i + 1][j + 1])
            {
                neighbors++;
            }

            //assign life value based on number of neighbors
            if (neighbors <= 1 || neighbors > 3)
            {
                //cell is dead
                tempWorld[i][j] = false;
            }
            if (neighbors == 3 )
            {
                //cell is alive
                tempWorld[i][j] = true;
            }
        }
    }

    //copy the updated temp array to the game world for the next generation
    copyWorld(tempWorld, gameWorld);
}

/****************************************************************************************
**Function: printWorld
**Description: Prints the current game world, only displaying the middle 20 x 40 part of
**the entire world array.
**Parameters: Oversized game world with "true" and "false" cells.
**Pre-conditions: gameWorld array contains game data of "true" and "false" cells from
**either initial pattern placement or previous generation simulation.
**Post-conditions: The visible world has been outputted with + designating alive cells
**and blank space designating a dead cell.
****************************************************************************************/
void printWorld(bool gameWorld[60][80])
{
    //adjust loop values by 20 for visible game window and oversized array
    for (int i = 20; i < 40; i++)
    {
        for (int j = 20; j < 60; j++)
        {
            if (gameWorld[i][j])
            {
                cout << "+";
            }
            else
            {
                cout << " ";
            }
        }

        cout << endl;
    }
}
