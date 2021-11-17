#pragma once
#include "CManager.h"

class CSnake
{
public:
	CSnake() {};
	CSnake(CManager& inGraphics);
	~CSnake() {};
	void spawn();
	void draw(bool& gameOver);
	void move(bool& gameOver);
	void changeDir(SDL_KeyCode newDir);
	void handleCollisions(bool& gameOver);

	//Public for CFood workaround... 
	SDL_Rect snakeArr[CELL_COUNT];

private:
	int dx = CELL_WIDTH;
	int dy = 0;
	CManager graphics;
	int gameOverTicker = 0;
};


