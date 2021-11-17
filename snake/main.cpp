#pragma once
#include "CBoard.h"

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
CSnake player;

//Initialize *AFTER* init() so that it can pull from CManager
CBoard game;

//Initialize *AFTER* init() so that it can pull from CManager
CFood food;

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

	//Initialize Snake
	player = CSnake(graphics);
	player.spawn();

	//Initialize GameBoard
	game = CBoard(graphics);

	//Initialize food
	food = CFood(graphics);
	food.spawnFood(player);

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
			game.handleInput(player, quit);

			//Move snake
			player.move(gameOver);

			//Draw snake
			player.draw(gameOver);

			//Draw food
			food.drawFood();

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