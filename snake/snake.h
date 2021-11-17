#pragma once
#include "CManager.h"

class Snake
{
public:
	Snake() {};
	Snake(CManager& inGraphics);
	~Snake() {};
	void spawn();
	void draw(bool& gameOver);
	void move(bool& gameOver);
	void changeDir(SDL_KeyCode newDir);
	void handleCollisions(bool& gameOver);

private:
	SDL_Rect snakeArr[CELL_COUNT];
	int dx = CELL_WIDTH;
	int dy = 0;
	CManager graphics;
	int gameOverTicker = 0;
};


