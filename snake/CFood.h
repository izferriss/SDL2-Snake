#pragma once
#include "CManager.h"
#include "CSnake.h"

class CFood
{
public:
	CFood() {};
	CFood(CManager& inGraphics);
	~CFood() {};

	void spawnFood(CSnake snake);
	void drawFood();
private:
	SDL_Rect food;
	int w = CELL_WIDTH;
	int h = CELL_HEIGHT;

	CManager graphics;
};
