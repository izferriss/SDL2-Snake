#pragma once
#include "game.h"

/*////////////////////////////////////////////////////////
// Variables /////////////////////////////////////////////
////////////////////////////////////////////////////////*/

//SDL_Renderer and SDL_Window mgr (get/set)
CManager graphics;

//Initialized in init() and set in CManager
SDL_Renderer* gRenderer;
//Initialized in init() and set in CManager
SDL_Window* gWindow;

//Initialize *AFTER* init() so that it can pull from CManager
CTexture gTextTexture;

//Initialize *AFTER* init() so that it can pull from CManager
CBoard game;

/*////////////////////////////////////////////////////////
// FLAGS /////////////////////////////////////////////////
////////////////////////////////////////////////////////*/

bool quit = false;
bool gameOver = false;

/*////////////////////////////////////////////////////////
// MAIN PROGRAM //////////////////////////////////////////
////////////////////////////////////////////////////////*/
int main(int argc, char* args[])
{
	//Initialize SDL
	graphics.init();

	//Initialize CTexture
	gTextTexture = CTexture(graphics);

	//Initialize GameBoard
	game = CBoard(graphics);
	game.spawnSnake();

	//Initialize food
	game.spawnFood();

	//Declare score
	//int score = 0;
	//std::string scoreString = "Score: " + std::to_string(score);

	//While the application is running
	while (!quit)
	{
		std::string score = game.getScoreString();

		if (!game.loadMessage(gTextTexture, score))
		{
			printf("Failed to load media!\n");
			quit = true;
		}
		else
		{
			//Clear screen to black
			SDL_SetRenderDrawColor(graphics.getRenderer(), 0, 0, 0, 255);
			SDL_RenderClear(graphics.getRenderer());

			//Handle input
			game.handleInput(quit);

			//Move snake
			game.moveSnake(gameOver);

			//Draw snake
			game.drawSnake(gameOver);

			//Draw food
			game.drawFood();

			//Draw walls to screen edges
			game.drawWalls();

			//Draw score
			gTextTexture.render((SCREEN_WIDTH / 2), 1, NULL, 0, NULL, SDL_FLIP_NONE);

			//Render to screen
			SDL_RenderPresent(graphics.getRenderer());

			//wait 100ms before next loop iteration
			SDL_Delay(100);
		}
	}

	//Clean up program and quit
	graphics.exit();
	return 0;
}