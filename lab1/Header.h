#pragma once

class Game {
public:
	Game();
	~Game();
	int GetNum(int i, int j);
	bool Check();
	bool Change(int i, int j);
	void GetMatrix();
	void Shaker(int iterations);
	bool operator == (const Game& b);
	bool operator != (const Game& b);
	int empty_point_i = 3;
	int empty_point_j = 3;
private:
	int matrix[4][4];
};

