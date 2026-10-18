/* !
\file
\brief Файл с описанием всех методов класса Game.
*/

#include "Header.h"
#include <cstdlib> 
#include <iostream>
#include <iomanip>

using namespace std;

/*!
Конструктор класса Game, который создает матрицу заполненную числами в правильном порядке.
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
/// Деструктор для очистки ресурсов
Game::~Game() {}
/*!
Данный метод отвечает за получение значения клетки ([]).
\param[in] i Номер строки.
\param[in] j Номер столбца.
\return Целое число соответствующего данному номеру строки и столбца.
*/
int Game::GetNum(int i, int j) {
	return matrix[i][j];
}
/*!
Данный метод отвечает за перестановку клеток.
\param[in] i Номер строки.
\param[in] j Номер столбца.
\return Возвращает булевое значение в зависимости от того удалось ли выполнить операцию.
*/
bool Game::Change(int i, int j) {
	if (i > -1 && j < 4) {
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
/// Выводит матрицу игрового поля на экран (интерфейс)
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
Данный метод отвечает за случайное расположение номеров перед началом игры. Он в рандомном порядке (в соответствии с правилами игры) 
переставляет пустую клетку, что гарантирует решаемость головоломки.
\param[in] iterations Количество перестановок.
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
Оператор проверки равенства двух игровых состояний.
\param[in] b Ссылка на объект класса с которым мы хотим провести сравнение.
\return Возвращает булевое значение в зависимости от того равны ли два объекта или нет.
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
Оператор проверки неравенства двух игровых состояний.
\param[in] b Ссылка на объект класса с которым мы хотим провести сравнение.
\return Возвращает булевое значение в зависимости от того неравны ли два объекта или нет.
*/
bool Game::operator != (const Game& b) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (this->matrix[i][j] == b.matrix[i][j]) {
				continue;
			}
			else { return true; }
		}
	}
	return false;
}
