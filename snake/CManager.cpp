#include "CManager.h"

bool CManager::init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		//Error message and set flag
		printf("Failed to initialize SDL! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Initialize SDL_ttf
		if (TTF_Init() == -1)
		{
			printf("SDL_ttf could not intialize! SDL_ttf Error: %s\n", TTF_GetError());
			success = false;
		}
		else
		{
			//Create window
			mWindow = SDL_CreateWindow("SDL Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
			//Set window to borderless
			SDL_SetWindowBordered(mWindow, SDL_FALSE);
			if (mWindow == NULL)
			{
				//Error message and set flag
				printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Create renderer
				mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
				if (mRenderer == NULL)
				{
					//Error message and set flag
					printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
					success = false;
				}
				else
				{
					//
				}
			}
		}
	}

	return success;
}
void CManager::exit()
{
	//Destroy renderer
	if (mRenderer)
	{
		SDL_DestroyRenderer(mRenderer);
		mRenderer = NULL;
	}

	//Destroy window
	if (mWindow)
	{
		SDL_DestroyWindow(mWindow);
		mWindow = NULL;
	}

	//Quit subsystems
	SDL_Quit();
	IMG_Quit();
	TTF_Quit();
}