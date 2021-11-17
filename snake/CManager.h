#pragma once
#include <SDL.h>
#include <stdio.h>
#include <cstdint>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <string>
#include <cmath>

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
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
public:
	CManager() {};
	~CManager() {};

	SDL_Window* getWindow() { return mWindow; }
	SDL_Renderer* getRenderer() { return mRenderer; }
	void setWindow(SDL_Window*& newWindow) { mWindow = newWindow; }
	void setRenderer(SDL_Renderer*& newRenderer) { mRenderer = newRenderer; }
	bool init();
	void exit();
};

