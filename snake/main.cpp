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
CTexture gScoreTextTexture;
CTexture gHiScoreTextTexture;
CTexture gFPSTextTexture;

//Initialize *AFTER* init() so that it can pull from CManager
CBoard game;

std::string score;
std::string fps;
std::string hiScore;


/*////////////////////////////////////////////////////////
// MAIN PROGRAM //////////////////////////////////////////
////////////////////////////////////////////////////////*/
int main(int argc, char* args[])
{
	//Initialize SDL
	graphics.init();

	//Initialize text textures
	gScoreTextTexture = CTexture(graphics);
	gHiScoreTextTexture = CTexture(graphics);
	gFPSTextTexture = CTexture(graphics);

	//Initialize GameBoard
	game = CBoard(graphics);
	game.spawnSnake();

	//Initialize food
	game.spawnFood();

	//Get highscore
	game.getHighScore();

	//While not QUIT
	while (game.getState() != 4)
	{
		score = game.getScoreString();
		fps = game.getFPSString();
		hiScore = game.getHighScoreString();

		Uint64 start = SDL_GetPerformanceCounter();

		if (!game.loadMessage(gScoreTextTexture, score))
		{
			printf("Failed to load score texture!\n");
			game.setState(4);
		}
		else
		{
			if (!game.loadMessage(gFPSTextTexture, fps))
			{
				printf("Failed to load FPS texture!\n");
				game.setState(4);
			}

			else
			{
				if (!game.loadMessage(gHiScoreTextTexture, hiScore))
				{
					printf("Failed to load hiScore texture!\n");
					game.setState(4);
				}
				else
				{
					//Clear screen to black
					SDL_SetRenderDrawColor(graphics.getRenderer(), 0, 0, 0, 255);
					SDL_RenderClear(graphics.getRenderer());

					//Handle input
					game.handleInput();

					//Move snake
					game.moveSnake();

					//Draw snake
					game.drawSnake();

					//Draw food
					game.drawFood();

					//Draw walls to screen edges
					game.drawWalls();

					//Draw score
					gScoreTextTexture.render(CELL_WIDTH, 1, NULL, 0, NULL, SDL_FLIP_NONE);

					//Draw hiScore
					gHiScoreTextTexture.render(SCREEN_WIDTH / 2, 1, NULL, 0, NULL, SDL_FLIP_NONE);

					//Draw fps
					gFPSTextTexture.render(SCREEN_WIDTH - (CELL_WIDTH * 8), 1, NULL, 0, NULL, SDL_FLIP_NONE);

					//Render to screen
					SDL_RenderPresent(graphics.getRenderer());

					//Time since work done
					Uint64 end = SDL_GetPerformanceCounter();
					float elapsed = (end - start) / (float)SDL_GetPerformanceFrequency();
					game.updateFPSString(1.0f / elapsed);

					gScoreTextTexture.free();
					gFPSTextTexture.free();
					gHiScoreTextTexture.free();
				}
			}
		}
	}

	if (game.getState() == 4)
	{
		//Clean up program and quit
		graphics.exit();
	}
	return 0;
}