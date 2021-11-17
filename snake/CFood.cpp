#include "CFood.h"

CFood::CFood(CManager& inGraphics)
{
	graphics = inGraphics;
}

void CFood::spawnFood(CSnake snake)
{
	//Set width and height
	food.w = w;
	food.h = h;

	//Generate a random x/y location within the walls of the board
	food.x = (rand() % (((SCREEN_WIDTH - CELL_WIDTH - WALL_THICKNESS) / CELL_WIDTH) + 1)*CELL_WIDTH);
	food.y = (rand() % (((SCREEN_HEIGHT - CELL_HEIGHT - WALL_THICKNESS) / CELL_HEIGHT) + 1)*CELL_HEIGHT);

	//Account for top wall difference
	if (food.y < (WALL_THICKNESS * 2))
	{
		food.y = (WALL_THICKNESS * 2);
	}

	//Only spawn food if it does not touch the snake
	for (int i = 0; i < sizeof(snake.snakeArr) / sizeof(snake.snakeArr[0]); i++)
	{
		//Exit loop when at the end of active snake elements (w=0)
		if (snake.snakeArr[i].w == 0)
		{
			break;
		}
		if(snake.snakeArr[i].x == food.x && snake.snakeArr[i].y == food.y)
		{
			spawnFood(snake);
			break;
		}
	}
}

void CFood::drawFood()
{
	//Make the food yellow
	SDL_SetRenderDrawColor(graphics.getRenderer(), 255, 255, 0, 255);
	SDL_RenderFillRect(graphics.getRenderer(), &food);
}