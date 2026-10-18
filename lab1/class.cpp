/*!
\file
\brief File containing the implementation of all Game class methods.
*/

#include "Header.h"
#include <cstdlib> 
#include <iostream>
#include <iomanip>

using namespace std;

/*!
Constructor of the Game class that creates a matrix filled with numbers in the correct order.
*/
Game::Game(): matrix{} {
	int count = 1;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == 3 && j == 3) {
				matrix[i][j] = 0;
			}
			else {
				matrix[i][j] = count;
					count++;
			}
		}
	}
	empty_point_i = 3;
	empty_point_j = 3;
}
/// Destructor for resource cleanup.
Game::~Game() {}
/*!
This method returns the value of a specific tile.
\param[in] i Row index.
\param[in] j Column index.
\return Integer value corresponding to the given row and column.
*/
int Game::GetNum(int i, int j) {
	return matrix[i][j];
}
/*!
This method handles moving/swapping the tiles.
\param[in] i Row index of the target tile.
\param[in] j Column index of the target tile.
\return Returns a boolean value indicating whether the move was successful.
*/
bool Game::Change(int i, int j) {
	if (i >= 0 && i < 4 && j >= 0 && j < 4) {
		if ((abs(i - empty_point_i) == 1 && empty_point_j - j == 0) || (abs(j - empty_point_j) == 1 && empty_point_i - i == 0)) {
			int temp = matrix[i][j];
			matrix[i][j] = matrix[empty_point_i][empty_point_j];
			matrix[empty_point_i][empty_point_j] = temp;
			empty_point_i = i;
			empty_point_j = j;
			return 1;
		}
		else { return 0; }
	}
	else { return 0; }
}
/// Prints the game board matrix to the screen (UI).
void Game::GetMatrix() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (matrix[i][j] == 0) {
				cout << setw(3) << "*" << "|";
				continue;
			}
			cout << setw(3) << matrix[i][j] << "|";
		}
		cout << endl << "----------------" << endl;
	}
}
/*!
This method randomly shuffles the numbers before starting the game.
It moves the empty cell following the rules of the game, which guarantees that the puzzle remains solvable.
\param[in] iterations Number of shuffles/moves.
*/
void Game::Shaker(int iterations) {
	int point_i = 3;
	int point_j = 3;
	for (int i = 0; i < iterations; i++) {
		int direction = rand() % 4;

		int next_i = point_i;
		int next_j = point_j;

		if (direction == 0) { next_i--; }
		else if (direction == 1) { next_j++; }
		else if (direction == 2) { next_i++; }
		else if (direction == 3) { next_j--; }

		if (next_i < 4 && next_i > -1 && next_j < 4 && next_j > -1) {
			int temp = matrix[next_i][next_j];
			matrix[next_i][next_j] = matrix[point_i][point_j];
			matrix[point_i][point_j] = temp;
		}
		else { continue; }
		point_i = next_i;
		point_j = next_j;
	}
	empty_point_i = point_i;
	empty_point_j = point_j;
}
/*!
Equality operator to compare two game states.
\param[in] b Reference to the Game object to compare with.
\return Returns true if both objects are identical, otherwise false.
*/
bool Game::operator == (const Game& b) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (this->matrix[i][j] == b.matrix[i][j]) {
				continue;
			}
			else { return false; }
		}
	}
	return true;
}
/*!
Inequality operator to compare two game states.
\param[in] b Reference to the Game object to compare with.
\return Returns true if objects are different, otherwise false.
*/
bool Game::operator != (const Game& b) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (this->matrix[i][j] != b.matrix[i][j]) {
				return true;
			}
			else { continue; }
		}
	}
	return false;
}
