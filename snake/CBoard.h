#pragma once
#include "CManager.h"
#include "CTexture.h"
#include "CFood.h"

class CBoard
{
public:
	CBoard() {};
	CBoard(CManager& inGraphics);
	//Draws walls for the screen
	void drawWalls();
	//Handle input
	void handleInput(CSnake& player, bool &quitFlag);
	//Loads message to font texture
	bool loadMessage(CTexture& gTextTexture, std::string& message);
	//Gets score string
	std::string getScoreString() { return scoreString; }
	//Gets score
	int getScore() { return score; }
	//Sets score
	void setScore(int input) { score = input; }


private:
	//GraphicsMgr
	CManager graphics;
	//Score
	int score = 0;
	//Score string
	std::string scoreString;
	//SDL Font placeholder
	TTF_Font* gFont = NULL;
};

