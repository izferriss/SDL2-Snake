#include "CManager.h"

class CTexture
{
public:
	//Initializes variables
	CTexture();
	CTexture(CManager& inGraphics);
	//Deallocates memory
	~CTexture();
	//Loads image at a specific path
	bool loadFromFile(std::string path);
	//Deallocates texture
	void free();
	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);
	//Set blending
	void setBlendMode(SDL_BlendMode blending);
	//Set alpha modulation
	void setAlpha(Uint8 alpha);
	//Renders texture at a given point
	void render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip);
	//Creates image from font string
	bool loadFromRenderedText(TTF_Font* gFont, std::string textureText, SDL_Color textColor);
	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	SDL_Texture* mTexture;
	CManager graphics;
	int mWidth;
	int mHeight;
};