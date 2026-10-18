/*!
\file
\brief File containing the user interface implementation.

This file contains the console menu logic for the "15-puzzle" game,
user input handling, and gameplay management.
*/
#include <iostream>
#include "Header.h"
#include <ctime>

using namespace std;
/*!
\brief Main function of the program that controls the game loop.
\details Before starting, the rules of the game are printed to the console.
Then the user can select the difficulty level (easy, medium, hard).
If the user enters an invalid choice, the medium level is set automatically.

In the main loop, the game menu is displayed where the user chooses an action:
- Move a tile (the program requests a row and a column; if the move is invalid, the action is ignored).
- Get a tile value by its coordinates.
- Exit the game.
*/

int main() {
	srand(time(0));

	cout << "Hi! This is a puzzle game called '15-puzzle'" << endl;
	cout << "Here are its rules:" << endl
		<< "1. The game has a 4x4 grid. There are 15 numbers (1 to 15) and one empty space (shown as 0)." << endl
		<< "2. You cannot move numbers diagonally or jump over them."
		<< " You can only slide a number into the empty space if it is right   next to it (up, down, left, or right)." << endl
		<< "3. Put all the mixed numbers back in the right order." << endl
		<< "4. Line up the numbers from 1 to 15 (from left to right, top to bottom). The empty space (0) must be in the bottom-right   corner." << endl << endl;
	cout << "Before starting, please select the difficulty level: " << endl
		<< "1. Easy" << endl
		<< "2. Medium" << endl
		<< "3. Hard" << endl;
	int level;
	int iter = 100;
	cin >> level;
	if (level == 1) {
		iter = 30;
	}
	else if (level == 2) {
		iter = 100;
	}
	else if (level == 3) {
		iter = 200;
	}
	else {
		cout << "Invalid input! Defaulting to medium difficulty." << endl;
	}
	cout << endl;
	cout << "LET'S START!" << endl;
	Game winningGrid;
	Game currentGrid;
	currentGrid.Shaker(iter);
	bool cont = true;
	int n = 0;
	while (cont) {
		cout << endl;
		currentGrid.GetMatrix();
		cout << endl;
		cout << "MENU:" << endl
			<< "1. Move tiles" << endl
			<< "2. Get tile value" << endl
			<< "3. Exit" << endl;
		cin >> n;
		if (n < 1 || n > 3) {
			continue;
		}
		switch (n) {
		case(1): {
			int i;
			int j;
			cout << "Type the number of the row: ";
			cin >> i;
			cout << "Type the number of the collumn: ";
			cin >> j;
			if (currentGrid.Change(i - 1, j - 1)) {
				if (currentGrid == winningGrid) {
					cont = false;
					cout << "VICTORY!!!" << endl;
				}
			}
			else { continue; }
			break;
		}
		case(2): {
			int i;
			int j;
			cout << "Type the number of the row: ";
			cin >> i;
			cout << "Type the number of the collumn: ";
			cin >> j;
			if (i - 1 < 0 || i - 1 > 3 || j - 1 < 0 || j - 1 > 3) {
				continue;
			}
			cout << currentGrid.GetNum(i - 1, j - 1);
			break;
		}
		case(3): {
			cout << "END." << endl;
			cont = false;
			break;
		}
		}
	}
	return 0;
}