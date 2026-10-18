/*!
\file
\brief Header file containing the Game class declaration.

This file contains the declaration of the Game class, which combines
the game logic of the "15-puzzle" and interface methods.
*/

#pragma once

class Game {
public:
	/// Constructor that creates the initial state of the game.
	Game();
	/// Destructor for resource cleanup.
	~Game();
	/// Returns the element value by row index i and column index j.
	int GetNum(int i, int j);
	/// Moves a tile to the empty position if the move is valid.
	bool Change(int i, int j);
	/// Prints the game board matrix to the screen (UI).
	void GetMatrix();
	/// Shuffles the game board a specified number of times.
	void Shaker(int iterations);
	/// Equality operator to compare two game states.
	bool operator == (const Game& b);
	/// Equality operator to compare two game states.
	bool operator != (const Game& b);
private:
	/// Row index of the empty cell (0) on the board.
	int empty_point_i;
	/// Column index of the empty cell (0) on the board.
	int empty_point_j;
	/// 4x4 matrix representing the puzzle grid.
	int matrix[4][4];
};

