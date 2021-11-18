#pragma once
#include "game.h"

CBoard::CBoard(CManager& inGraphics)
{
	graphics = inGraphics;
	updateScoreString();
}
void CBoard::handleInput(bool &quitFlag)
{
	//Event handler
	SDL_Event e;

	//Handle events on queue
	while (SDL_PollEvent(&e) != 0)
	{
		//If quit event
		if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
		{
			quitFlag = true;
		}

		if (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_UP))
		{
			changeDirSnake(SDLK_UP);
		}

		if (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_DOWN))
		{
			changeDirSnake(SDLK_DOWN);
		}

		if (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_LEFT))
		{
			changeDirSnake(SDLK_LEFT);
		}

		if (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_RIGHT))
		{
			changeDirSnake(SDLK_RIGHT);
		}
	}//end event handler loop
}
//////////////////////////////////////////////////////////
void CBoard::drawWalls()
{
	//Define wall color
	SDL_SetRenderDrawColor(graphics.getRenderer(), 210, 209, 205, 255); //gray

	//Rectangle for drawing
	SDL_Rect block;

	//left wall
	//x = left edge, y = top edge
	block.x = 0;
	block.y = WALL_THICKNESS; //to allow for WALL_THICKNESS worth of space at the top.
	block.w = WALL_THICKNESS;
	block.h = SCREEN_HEIGHT;
	SDL_RenderFillRect(graphics.getRenderer(), &block);

	//right wall
	//x = right edge - wall thickness, y = top edge
	block.x = SCREEN_WIDTH - WALL_THICKNESS;
	SDL_RenderFillRect(graphics.getRenderer(), &block);

	//top wall
	//x = left edge, y = top edge
	block.x = 0;
	block.w = SCREEN_WIDTH;
	block.h = WALL_THICKNESS;
	SDL_RenderFillRect(graphics.getRenderer(), &block);

	//bottom wall
	//x = left edge, y = bottom edge - wall thickness
	block.y = SCREEN_HEIGHT - WALL_THICKNESS;
	SDL_RenderFillRect(graphics.getRenderer(), &block);
}
//////////////////////////////////////////////////////////
bool CBoard::loadMessage(CTexture &gTextTexture, std::string& message)
{
	//Loading success flag
	bool success = true;

	//Open the font
	gFont = TTF_OpenFont("font/arial.ttf", 16);

	{
		if (gFont == NULL)
		{
			printf("Failed to load Arial font! SDL_ttf Error: %s\n", TTF_GetError());
			success = false;
		}
		else
		{
			//Render text
			if (!gTextTexture.loadFromRenderedText(gFont, message, { 0xFF,0xFF,0xFF,0xFF }))
			{
				printf("Failed to render text texture!\n");
				success = false;
			}
		}

		return success;
	}
}
//////////////////////////////////////////////////////////
void CBoard::updateScoreString()
{
	scoreString = "Score: " + std::to_string(score);
}
//////////////////////////////////////////////////////////
void CBoard::spawnFood()
{
	//Set width and height
	food.w = foodW;
	food.h = foodH;

	//Generate a random x/y location within the walls of the board
	food.x = (rand() % (((SCREEN_WIDTH - CELL_WIDTH - WALL_THICKNESS) / CELL_WIDTH) + 1)*CELL_WIDTH);
	food.y = (rand() % (((SCREEN_HEIGHT - CELL_HEIGHT - WALL_THICKNESS) / CELL_HEIGHT) + 1)*CELL_HEIGHT);

	//Account for top wall difference
	if (food.y < (WALL_THICKNESS * 2))
	{
		food.y = (WALL_THICKNESS * 2);
	}

	//Only spawn food if it does not touch the snake
	for (int i = 0; i < sizeof(snakeArr) / sizeof(snakeArr[0]); i++)
	{
		//Exit loop when at the end of active snake elements (w=0)
		if (snakeArr[i].w == 0)
		{
			break;
		}
		if (snakeArr[i].x == food.x && snakeArr[i].y == food.y)
		{
			spawnFood();
			break;
		}
	}
}
//////////////////////////////////////////////////////////
void CBoard::drawFood()
{
	//Make the food yellow
	SDL_SetRenderDrawColor(graphics.getRenderer(), 255, 255, 0, 255);
	SDL_RenderFillRect(graphics.getRenderer(), &food);
}
//////////////////////////////////////////////////////////
void CBoard::spawnSnake()
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
void CBoard::drawSnake(bool& gameOver)
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
void CBoard::moveSnake(bool& gameOver)
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
	snakeArr[0].x = snakeArr[1].x + snakeDX;
	snakeArr[0].y = snakeArr[1].y + snakeDY;
	snakeArr[0].w = CELL_WIDTH;
	snakeArr[0].h = CELL_HEIGHT;

	if (food.x == snakeArr[0].x && food.y == snakeArr[0].y)
	{
		spawnFood();
		setScore(score += 1);
		updateScoreString();
	}
	else
	{
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
	}

	handleCollisions(gameOver);
}
//////////////////////////////////////////////////////////
void CBoard::changeDirSnake(SDL_KeyCode newDir)
{
	//figure out currDir
	int up = snakeDY == -CELL_HEIGHT;
	int down = snakeDY == CELL_HEIGHT;
	int left = snakeDX == -CELL_WIDTH;
	int right = snakeDX == CELL_WIDTH;

	//change to up if not going down
	if (newDir == SDLK_UP && !down)
	{
		snakeDX = 0;
		snakeDY = -CELL_HEIGHT;
	}

	//change do down if not going up
	if (newDir == SDLK_DOWN && !up)
	{
		snakeDX = 0;
		snakeDY = CELL_HEIGHT;
	}

	//change to left if not going right
	if (newDir == SDLK_LEFT && !right)
	{
		snakeDX = -CELL_WIDTH;
		snakeDY = 0;
	}

	//change to right if not going left
	if (newDir == SDLK_RIGHT && !left)
	{
		snakeDX = CELL_WIDTH;
		snakeDY = 0;
	}
}
//////////////////////////////////////////////////////////
void CBoard::handleCollisions(bool &gameOver)
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
