/****************************************************************************************
**Description: This program allows the user to place their choice of 3 patterns on a
**game world array and then executes the Conway's Game of Life rule set for a user
**determined number of generations.
**
**Input: Pattern selection, placement of pattern, and number of generations to run.
**
**Output: Prints the pattern in the appropriate location for the number of generations
**specified.
****************************************************************************************/

#include <iostream>
#include <cstdlib> //used for clear screen
#include <unistd.h> //used for usleep
#include <limits> //used to eliminate character input
#include "board.h"

using std::cout;
using std::cin;
using std::endl;
using std::numeric_limits;
using std::streamsize;

void validate(int &input, int lowest, int highest);

int main()
{
    int patternChoice = 0; //user choice for pattern type
	int userRow = 0; //user specified row coordinate
	int userColumn = 0; //user specified column coordinate
    int generations = 0; //number of generations desired
    //the game world with 20 extra cells on every side with all cells initialized to false
    bool world[60][80] = {false};

    cout << endl << "This is Conway's Game of Life!!!" << endl << endl;
	cout << "What life pattern would you like to test?" << endl
         << "1 = Blinker" << endl
         << "2 = Glider" << endl
         << "3 = Gun" << endl << endl
         << "Selection: ";

	//gather and validate all the user input
	validate(patternChoice, 1, 3);

	cout << endl << "Please select a row for the center of the pattern (1-20): ";

	validate(userRow, 1, 20);

	cout << endl << "Please select a column for the center of the pattern (1-40): ";

	validate(userColumn, 1, 40);

	//change row and column value to compensate for extra 20 cells and to start at 1 not 0
	userRow = userRow + 19;
	userColumn = userColumn + 19;

	cout << endl << "How many generations do you want to simulate? ";

    validate(generations, 1, 10000000);

	//user chooses Blinker type
	if (patternChoice == 1)
    {
        //place blinker pattern where user has specified
        placeBlinker(world, userRow, userColumn);

        //run the animation until the generations reach zero
        do
        {
            //clear the screen
            system("clear");

            printWorld(world);

            //delay for smooth animation of world, got idea from class Q&A
            usleep(25000);

            //run the rules of the game to get the data for the next generation
            simulateWorld(world);

            generations--;

        } while (generations > 0);
    }

    //user chooses Glider type
    if (patternChoice == 2)
    {
        //place glider pattern where user has specified
        placeGlider(world, userRow, userColumn);

        //run the animation until the generations reach zero
        do
        {
            //clear the screen
            system("clear");

            printWorld(world);

            //delay for smooth animation of world, got idea from class Q&A
            usleep(25000);

            //run the rules of the game to get the data for the next generation
            simulateWorld(world);

            generations--;

        } while (generations > 0);
    }

    //user chooses Gun type
    if (patternChoice == 3)
    {
        //place glider pattern where user has specified
        placeGun(world, userRow, userColumn);

        //run the animation until the generations reach zero
        do
        {
            //clear the screen
            system("clear");

            printWorld(world);

            //delay for smooth animation of world, got idea from class Q&A
            usleep(25000);

            //run the rules of the game to get the data for the next generation
            simulateWorld(world);

            generations--;

        } while (generations > 0);
    }

    cout << "The simulation has ended. Have a nice day!" << endl;

    return 0;
}

/****************************************************************************************
**Function: validate
**Description: Validates the input to eliminate non number and out of range input.
**Parameters: User input passed by reference, low range and high range.
**Pre-conditions: None.
**Post-conditions: Input has been validated to be an integer in the correct range.
****************************************************************************************/
void validate(int &input, int lowest, int highest)
{
    while (!(cin >> input) || input < lowest || input > highest)
	{
		cout << "Your input was invalid! Please try again: ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
}
