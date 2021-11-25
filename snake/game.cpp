#pragma once
#include "game.h"

CBoard::CBoard(CManager& inGraphics)
{
	graphics = inGraphics;
	updateScoreString();
	updateFPSString(0);
	Game_State state{};
	currState = 0;
	state = NOT_PLAYING;

	//Seed random with currTime
	srand((unsigned)time(0));
}
//////////////////////////////////////////////////////////
void CBoard::setState(int inState)
{
	currState = inState;
}
//////////////////////////////////////////////////////////
void CBoard::handleInput()
{
	//Event handler
	SDL_Event e;

	//Handle events on queue
	while (SDL_PollEvent(&e) != 0)
	{
		//If quit event
		if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
		{
			setState(QUIT);
		}
		//Up / w
		if (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_w))
		{
			// start the game when first arrow key is pressed
			if (getState() == NOT_PLAYING)
			{
				setState(PLAYING);
			}
			changeDirSnake(SDLK_UP);
		}
		//Down / s
		if (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_s))
		{
			if (getState() == NOT_PLAYING)
			{
				setState(PLAYING);
			}
			changeDirSnake(SDLK_DOWN);
		}
		//Left / a
		if (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_a))
		{
			if (getState() == NOT_PLAYING)
			{
				setState(PLAYING);
			}
			changeDirSnake(SDLK_LEFT);
		}
		//Right / d
		if (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_d))
		{
			if (getState() == NOT_PLAYING)
			{
				setState(PLAYING);
			}
			changeDirSnake(SDLK_RIGHT);
		}

		// handle pause/unpause
		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE)
		{
			switch (currState)
			{
			case PAUSED:
				setState(PLAYING);
				break;
			case PLAYING:
				setState(PAUSED);
				break;
			case GAME_OVER:
				playAgain();
				break;
			default:
				break;
			}
		}

	}//end event handler loop
}
//////////////////////////////////////////////////////////
void CBoard::playAgain()
{
	snakeLength = DEFAULT_SNAKE_LENGTH;
	spawnSnake();
	changeDirSnake(SDLK_RIGHT);
	spawnFood();
	score = 0;
	setScore(score);
	updateScoreString();
	currState = NOT_PLAYING;
}
//////////////////////////////////////////////////////////
void CBoard::drawWalls()
{
	//Define wall color
	SDL_SetRenderDrawColor(graphics.getRenderer(), 210, 209, 205, 255); //gray

	//Rectangle for drawing
	SDL_Rect block{};

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

	{
		if (graphics.getFont() == NULL)
		{
			printf("Failed to load Arial font! SDL_ttf Error: %s\n", TTF_GetError());
			success = false;
		}
		else
		{
			//Render text
			if (!gTextTexture.loadFromRenderedText(graphics.getFont(), message, { 0xFF,0xFF,0xFF,0xFF }))
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

	//Account for placement within walls
	//Top wall
	if (food.y < (WALL_THICKNESS * 2))
	{
		food.y = (WALL_THICKNESS * 2);
	}
	//Bottom wall
	if (food.y > SCREEN_HEIGHT-WALL_THICKNESS)
	{
		food.y = SCREEN_HEIGHT - WALL_THICKNESS - CELL_HEIGHT;
	}
	//Left wall
	if (food.x < WALL_THICKNESS)
	{
		food.x = WALL_THICKNESS;
	}
	//Right wall
	if (food.x > SCREEN_WIDTH - WALL_THICKNESS)
	{
		food.x = SCREEN_WIDTH - WALL_THICKNESS - CELL_WIDTH;
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
	dirCanChange = true;

	//Make all snake parts empty
	for (int i = 0; i < sizeof(snakeArr) / sizeof(snakeArr[0]); i++)
	{
		snakeArr[i].x = 0;
		snakeArr[i].y = 0;
		snakeArr[i].w = 0;
		snakeArr[i].h = 0;
	}

	//The first element in the array is the head
	snakeHead.x = SNAKE_START_X;
	snakeHead.y = SNAKE_START_Y;
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
void CBoard::drawSnake()
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
		if (currState == GAME_OVER)
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
	if (currState == GAME_OVER)
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
void CBoard::moveSnake()
{

	//Don't move if not playing
	if (currState != PLAYING)
	{
		return;
	}

	//Get next position for input handling
	snakeHead.x += snakeDX;
	snakeHead.y += snakeDY;

	//Did snake head enter a new cell? check against first member of snakeArr
	int oldX = (int)round(snakeArr[0].x / CELL_WIDTH) * CELL_WIDTH;
	int newX = (int)round(snakeHead.x / CELL_WIDTH) * CELL_WIDTH;
	int oldY = (int)round(snakeArr[0].y / CELL_HEIGHT) * CELL_HEIGHT;
	int newY = (int)round(snakeHead.y / CELL_HEIGHT) * CELL_HEIGHT;
	 
	if (oldX == newX && oldY == newY)
	{
		//Snake hasn't actually moved yet -- ignore input
		return;
	}

	//shift all elements right to make room for new head at the beginning
	for (int i = snakeLength - 1; i >= 0; i--)
	{
		snakeArr[i] = snakeArr[i - 1];
	}

	//insert the head's new position at the beginning of snakeArr
	snakeArr[0].x = newX;
	snakeArr[0].y = newY;
	snakeArr[0].w = CELL_WIDTH;
	snakeArr[0].h = CELL_HEIGHT;

	//If snake head interacts with food
	if (food.x == snakeArr[0].x && food.y == snakeArr[0].y)
	{
		//Spawn new food
		spawnFood();

		//Update snakeLength
		snakeLength++;

		//Increase snakeSpeed
		snakeSpeed += .1f;

		//Update score int
		setScore(score += 1);

		//Update score string
		updateScoreString();

		//Compare hiScore
		compareScores();

	}
	//Move the snake
	else
	{
		//remove tail by finding the last inactive element in snakeArr
		//then zero out the one before it
		for (int i = snakeLength; i < snakeLength; i++)
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

		dirCanChange = true;
		if (skippedDir != SDLK_ESCAPE)
		{
			changeDirSnake(skippedDir);
			skippedDir = SDLK_ESCAPE;
		}
	}

	handleCollisions();
}
//////////////////////////////////////////////////////////
void CBoard::changeDirSnake(SDL_KeyCode newDir)
{
	//figure out currDir
	int up = snakeDY == -snakeSpeed;
	int down = snakeDY == snakeSpeed;
	int left = snakeDX == -snakeSpeed;
	int right = snakeDX == snakeSpeed;

	if (!dirCanChange)
	{
		skippedDir = newDir;
		return;
	}
	//change to up if not going down
	if (newDir == SDLK_UP && !down)
	{
		snakeDX = 0;
		snakeDY = -snakeSpeed;
		dirCanChange = false;
	}

	//change do down if not going up
	if (newDir == SDLK_DOWN && !up)
	{
		snakeDX = 0;
		snakeDY = snakeSpeed;
		dirCanChange = false;
	}

	//change to left if not going right
	if (newDir == SDLK_LEFT && !right)
	{
		snakeDX = -snakeSpeed;
		snakeDY = 0;
		dirCanChange = false;
	}

	//change to right if not going left
	if (newDir == SDLK_RIGHT && !left)
	{
		snakeDX = snakeSpeed;
		snakeDY = 0;
		dirCanChange = false;
	}
}
//////////////////////////////////////////////////////////
void CBoard::handleCollisions()
{
	//Did it run into itself?
	for (int i = 1; i < sizeof(snakeArr) / sizeof(snakeArr[0]); i++)
	{
		//exit loop when at the end of the active elements of the snake body
		if (snakeArr[i].w == 0)
		{
			break;
		}
		//check if the head has run into active body elements
		if (snakeArr[0].x == snakeArr[i].x && snakeArr[0].y == snakeArr[i].y)
		{
			setState(GAME_OVER);
			compareScores();
			writeHighScore();
			return;
		}

		//Did it hit a wall?
		if ((snakeArr[0].x <= WALL_THICKNESS - 1) || (snakeArr[0].x >= SCREEN_WIDTH - WALL_THICKNESS + 1) || (snakeArr[0].y <= (WALL_THICKNESS * 2) - 1) || (snakeArr[0].y >= SCREEN_HEIGHT - WALL_THICKNESS + 1))
		{
			setState(GAME_OVER);
			compareScores();
			writeHighScore();
			return;
		}
	}
}
//////////////////////////////////////////////////////////
void CBoard::updateFPSString(float fps)
{
	fpsString = "FPS: " + std::to_string(fps);
}
//////////////////////////////////////////////////////////
void CBoard::compareScores()
{
	if (score > highScore)
	{
		setHighScore(score);
	}
	else
	{
		//do nothing
	}
}
//////////////////////////////////////////////////////////
void CBoard::getHighScore()
{
	std::ifstream myfile(hiScoreFile);
	if (!myfile)
	{
		printf("\"%s\" could not be opened!", hiScoreFile);
		setState(QUIT);

	}
	else
	{
		while (myfile >> highScore)
		{
			setHighScore(highScore);
		}

		myfile.close();
	}
}
//////////////////////////////////////////////////////////
void CBoard::setHighScore(int inHighScore)
{
	highScore = inHighScore;
}
//////////////////////////////////////////////////////////
void CBoard::writeHighScore()
{
	std::ofstream myfile(hiScoreFile, std::ofstream::trunc);
	if (!myfile)
	{
		printf("\"%s\" could not be opened!", hiScoreFile);
		setState(QUIT);

	}
	else
	{
		myfile << highScore;
		myfile.close();
	}
}
//////////////////////////////////////////////////////////
std::string CBoard::getHighScoreString()
{
	highScoreString = "High Score: " + std::to_string(highScore);
	return highScoreString;
}
