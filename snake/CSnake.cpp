#include "CSnake.h"

CSnake::CSnake(CManager& inGraphics)
{
	//Allows CManager's renderer to be used
	//graphics.getRenderer() to render
	graphics = inGraphics;
}
//////////////////////////////////////////////////////////
void CSnake::spawn()
{
	//Make all snake parts empty
	for (int i = 0; i < sizeof(snakeArr) / sizeof(snakeArr[0]); i++)
	{
		snakeArr[i].x = 0;
		snakeArr[i].y = 0;
		snakeArr[i].w = 0;
		snakeArr[i].h = 0;
	}

	//The first element in the array is the head
	snakeArr[0].x = SNAKE_START_X;
	snakeArr[0].y = SNAKE_START_Y;
	snakeArr[0].w = CELL_WIDTH;
	snakeArr[0].h = CELL_HEIGHT;

	//Create rest of snake
	for (int i = 1; i < DEFAULT_SNAKE_LENGTH; i++)
	{
		//Give snake body cell same attributes as the head
		snakeArr[i] = snakeArr[0];
		//Make the body part appear x cell width's away from the head
		snakeArr[i].x = snakeArr[0].x - (CELL_WIDTH * i);
	}
}
//////////////////////////////////////////////////////////
void CSnake::draw(bool& gameOver)
{
	//Draw snake body
 	for (int i = 1; i < (sizeof(snakeArr) / sizeof(snakeArr[0])); i++)
	{
		//Only draw elements that have a width
		if (snakeArr[i].w == 0)
		{
			break;
		}

		//Color differently if game over
		if (gameOver)
		{
			//Red if dead
			SDL_SetRenderDrawColor(graphics.getRenderer(), 255, 0, 0, 255);//red
			SDL_RenderFillRect(graphics.getRenderer(), &snakeArr[i]);
		}
		else
		{
			//Green if you're good at this game
			SDL_SetRenderDrawColor(graphics.getRenderer(), 0, 128, 0, 255);//green
			SDL_RenderFillRect(graphics.getRenderer(), &snakeArr[i]);
		}

		//Create a black border around each snake body cell
		SDL_SetRenderDrawColor(graphics.getRenderer(), 0, 0, 0, 255);//black
		SDL_RenderDrawRect(graphics.getRenderer(), &snakeArr[i]);
	}

	//Draw snake head
	//Color differently if game over
	if (gameOver)
	{
		//Yellow flashies every now and again (not consistent?)
		if (gameOverTicker % 10 == 0)
		{
			SDL_SetRenderDrawColor(graphics.getRenderer(), 255, 255, 0, 255);//yellow
			SDL_RenderFillRect(graphics.getRenderer(), &snakeArr[0]);
			gameOverTicker = 1;
		}
		//Red if dead
		else
		{
			SDL_SetRenderDrawColor(graphics.getRenderer(), 255, 0, 0, 255);//red
			SDL_RenderFillRect(graphics.getRenderer(), &snakeArr[0]);
			gameOverTicker++;
		}
	}
	//Green if you're good at this game
	else
	{
		SDL_SetRenderDrawColor(graphics.getRenderer(), 0, 128, 0, 255);//green
		SDL_RenderFillRect(graphics.getRenderer(), &snakeArr[0]);
	}
}
//////////////////////////////////////////////////////////
void CSnake::move(bool& gameOver)
{
	if (gameOver)
	{
		return;
	}

	//shift all elements right to make room for new head at the beginning
	for (int i = sizeof(snakeArr) / sizeof(snakeArr[0]) - 1; i >= 0; i--)
	{
		snakeArr[i] = snakeArr[i - 1];
	}


	//insert the head's new position at the beginning of snakeArr
  	snakeArr[0].x = snakeArr[1].x + dx;
	snakeArr[0].y = snakeArr[1].y + dy;
	snakeArr[0].w = CELL_WIDTH;
	snakeArr[0].h = CELL_HEIGHT;

	//remove tail by finding the last inactive element in snakeArr
	//then zero out the one before it
	for (int i = DEFAULT_SNAKE_LENGTH; i < sizeof(snakeArr) / sizeof(snakeArr[0]); i++)
	{
		if (snakeArr[i].w == 0)
		{
			snakeArr[i - 1].x = 0;
			snakeArr[i - 1].y = 0;
			snakeArr[i - 1].w = 0;
			snakeArr[i - 1].h = 0;
			break;
		}
	}

	handleCollisions(gameOver);
}
//////////////////////////////////////////////////////////
void CSnake::changeDir(SDL_KeyCode newDir)
{
	//figure out currDir
	int up = dy == -CELL_HEIGHT;
	int down = dy == CELL_HEIGHT;
	int left = dx == -CELL_WIDTH;
	int right = dx == CELL_WIDTH;

	//change to up if not going down
	if (newDir == SDLK_UP && !down)
	{
		dx = 0;
		dy = -CELL_HEIGHT;
	}

	//change do down if not going up
	if (newDir == SDLK_DOWN && !up)
	{
		dx = 0;
		dy = CELL_HEIGHT;
	}

	//change to left if not going right
	if (newDir == SDLK_LEFT && !right)
	{
		dx = -CELL_WIDTH;
		dy = 0;
	}

	//change to right if not going left
	if (newDir == SDLK_RIGHT && !left)
	{
		dx = CELL_WIDTH;
		dy = 0;
	}
}
//////////////////////////////////////////////////////////
void CSnake::handleCollisions(bool &gameOver)
{
	//Did it run into itself?
	for (int i = 1; i < sizeof(snakeArr) / sizeof(snakeArr[0]); i++)
	{
		//exit loop when at the end of the active elements of the snake body
		if (snakeArr[i].w == 0)
		{
			break;
		}
		//check if the head has not run into active body elements
		if (snakeArr[0].x == snakeArr[i].x && snakeArr[0].y == snakeArr[i].y)
		{
			gameOver = true;
			return;
		}

		//Did it hit a wall?
		if ((snakeArr[0].x <= WALL_THICKNESS) || (snakeArr[0].x >= SCREEN_WIDTH - WALL_THICKNESS - CELL_WIDTH) || (snakeArr[0].y <= (WALL_THICKNESS * 2)) || (snakeArr[0].y >= SCREEN_HEIGHT - WALL_THICKNESS - CELL_HEIGHT))
		{
			gameOver = true;
		}
	}
}