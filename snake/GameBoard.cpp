#pragma once
#include "GameBoard.h"

GameBoard::GameBoard(CManager& inGraphics)
{
	graphics = inGraphics;
	scoreString = "Score: " + std::to_string(score);
}
void GameBoard::handleInput(Snake& player, bool &quitFlag)
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
			player.changeDir(SDLK_UP);
		}

		if (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_DOWN))
		{
			player.changeDir(SDLK_DOWN);
		}

		if (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_LEFT))
		{
			player.changeDir(SDLK_LEFT);
		}

		if (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_RIGHT))
		{
			player.changeDir(SDLK_RIGHT);
		}
	}//end event handler loop
}
//////////////////////////////////////////////////////////
void GameBoard::drawWalls()
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
bool GameBoard::loadMessage(CTexture &gTextTexture, std::string& message)
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
