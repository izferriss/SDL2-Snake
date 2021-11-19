#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>
#include <cstdint>
#include <string>
#include <cmath>
#include <fstream>


const int TARGET_FPS = 60;
const int MS_PER_UPDATE = 1000 / 30;
const int MS_PER_FRAME = 1000 / TARGET_FPS;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int WALL_THICKNESS = 20;
const int CELL_WIDTH = 20;
const int CELL_HEIGHT = 20;
const int CELL_COUNT = ((SCREEN_WIDTH - WALL_THICKNESS * 2)*(SCREEN_HEIGHT - WALL_THICKNESS * 2) / (CELL_WIDTH * CELL_HEIGHT));
const int SNAKE_START_X = 400;
const int SNAKE_START_Y = 200;
const int DEFAULT_SNAKE_LENGTH = 5;

class CManager
{
private:
	SDL_Window* mWindow = NULL;
	SDL_Renderer* mRenderer = NULL;
	//SDL Font placeholder
	TTF_Font* gFont = NULL;
public:
	CManager() {};
	~CManager() {};

	SDL_Window* getWindow() { return mWindow; }
	SDL_Renderer* getRenderer() { return mRenderer; }
	void setWindow(SDL_Window*& newWindow) { mWindow = newWindow; }
	void setRenderer(SDL_Renderer*& newRenderer) { mRenderer = newRenderer; }
	void setFont(TTF_Font*& newFont) { gFont = newFont; }
	TTF_Font* getFont() { return gFont; }


	bool init();
	void exit();
};

