#pragma once

using namespace std;

class Game {
public:
	Game(int num_mode);
	~Game();
	int GetNum(int i, int j);
	bool Check();
	bool Change(int i, int j, int(&matrix)[4][4]);
	void GetMatrix();
	int empty_point_i = 3;
	int empty_point_j = 3;
private:
	int matrix[4][4];
	void Shaker(int(&matrix)[4][4], int iterations);
};

