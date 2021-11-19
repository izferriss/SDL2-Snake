#pragma once
#include "CManager.h"
#include "CTexture.h"

class CBoard
{
public:
	//Initialize variables
	CBoard() {};
	//Initialize variables (with graphics)
	CBoard(CManager& inGraphics);
	//Draws walls for the screen
	void drawWalls();
	//Handle input
	void handleInput();
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
	//Update fps string
	void updateFPSString(float fps);
	//Get fps string
	std::string getFPSString() { return fpsString; }
	//Spawns food
	void spawnFood();
	//Draws food
	void drawFood();
	//Spawn snake
	void spawnSnake();
	//Draw snake
	void drawSnake();
	//Move snake
	void moveSnake();
	//Snake changes direction
	void changeDirSnake(SDL_KeyCode newDir);
	//Handle collisions
	void handleCollisions();
	//Get state
	int getState() { return currState; }
	//Set state
	void setState(int inState);
	//Restart game
	void playAgain();
	//Read from hiscore.dat and assign highscore
	void getHighScore();
	//Compare hiscores
	void compareScores();
	//Set highscore var
	void setHighScore(int inHighScore);
	//Write highscore to hiscore.dat
	void writeHighScore();
	//Get highscore string
	std::string getHighScoreString();
private:
	//GraphicsMgr
	CManager graphics;
	//Score
	int score = 0;
	//Score string
	std::string scoreString;
	//FPS string
	std::string fpsString;

	//High score
	int highScore;
	std::string hiScoreFile = "hiscore.dat";
	std::string highScoreString;

	//Food variables
	SDL_Rect food;
	int foodW = CELL_WIDTH;
	int foodH = CELL_HEIGHT;

	//Snake variables
	SDL_FPoint snakeHead;
	SDL_Rect snakeArr[CELL_COUNT];
	float snakeSpeed = .75f;
	float snakeDX = CELL_WIDTH;
	float snakeDY = 0;
	bool dirCanChange = true;
	int snakeLength = DEFAULT_SNAKE_LENGTH;
	SDL_KeyCode skippedDir;
	

	//Ticker for death aesthetics
	int gameOverTicker = 0;

	int currState;

	typedef enum Game_State {
		NOT_PLAYING = 0,
		PLAYING = 1,
		PAUSED = 2,
		GAME_OVER = 3,
		QUIT = 4
	} Game_State;

};

