#include "Header.h"
#include <cstdlib> // Для функций rand()
#include <iostream>



Game::Game(int num_mode): matrix{} {
	switch (num_mode) {
	case 1: {
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
		Shaker(matrix, 30);
		break;
	}
	case 2: {
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
		Shaker(matrix, 100);
		break;
	}
	case 3: {
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
		Shaker(matrix, 200);
		break;
	}
	}
}

Game::~Game() {}

int Game::GetNum(int i, int j) {
	return matrix[i][j];
}

bool Game::Check() {
	int count = 1;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == 3 && j == 3) {
				return true; 
			}
			if (matrix[i][j] == count) {
				count++;
				continue;
			}
			else { return 0; }
		}
	}
	return 1;
}

bool Game::Change(int i, int j, int(&matrix)[4][4]) {
	if (i > -1 && j < 4) {
		if (abs(i - empty_point_i) == 1 || abs(j - empty_point_j) == 1) {
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

void Game::GetMatrix() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			cout << matrix[i][j] << ' ';
		}
		cout << endl;
	}
}

void Game::Shaker(int(&matrix)[4][4], int iterations) {
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