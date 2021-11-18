#pragma once
#include "CManager.h"
#include "CTexture.h"

class CBoard
{
public:
	CBoard() {};
	CBoard(CManager& inGraphics);
	//Draws walls for the screen
	void drawWalls();
	//Handle input
	void handleInput(bool &quitFlag);
	//Loads message to font texture
	bool loadMessage(CTexture& gTextTexture, std::string& message);
	//Gets score string
	std::string getScoreString() { return scoreString; }
	//Gets score
	int getScore() { return score; }
	//Sets score
	void setScore(int input) { score = input; }
	//Update score string
	void updateScoreString();
	//Spawns food
	void spawnFood();
	//Draws food
	void drawFood();
	//Food accessor
	void spawnSnake();
	void drawSnake(bool& gameOver);
	void moveSnake(bool& gameOver);
	void changeDirSnake(SDL_KeyCode newDir);
	void handleCollisions(bool& gameOver);

private:
	//GraphicsMgr
	CManager graphics;
	//Score
	int score = 0;
	//Score string
	std::string scoreString;
	//SDL Font placeholder
	TTF_Font* gFont = NULL;


	SDL_Rect food;
	int foodW = CELL_WIDTH;
	int foodH = CELL_HEIGHT;

	SDL_Rect snakeArr[CELL_COUNT];
	int snakeDX = CELL_WIDTH;
	int snakeDY = 0;

	int gameOverTicker = 0;

};
/*
class CSnake
{
public:
	void spawn();
	void draw(bool& gameOver);
	void move(bool& gameOver, CBoard game, CSnake player);
	void changeDir(SDL_KeyCode newDir);
	void handleCollisions(bool& gameOver);
	//Get location of snake head
	SDL_Rect snakeHead() { return snakeArr[0]; }


	//Public for food workaround... 
	SDL_Rect snakeArr[CELL_COUNT];

private:

};
*/

