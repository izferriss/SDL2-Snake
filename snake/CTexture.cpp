#include "CTexture.h"

/*////////////////////////////////////////////////////////
// LTexture Class ////////////////////////////////////////
////////////////////////////////////////////////////////*/
CTexture::CTexture()
{
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}
CTexture::CTexture(CManager& inGraphics)
{
	//Allows CManager's renderer to be used
	//graphics.getRenderer() to render
	graphics = inGraphics;
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}
//////////////////////////////////////////////////////////
CTexture::~CTexture()
{
	free();
}
//////////////////////////////////////////////////////////
bool CTexture::loadFromFile(std::string path)
{
	//Get rid of pre-existing texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface = NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0, 0));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(graphics.getRenderer(), loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}
//////////////////////////////////////////////////////////
void CTexture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}
//////////////////////////////////////////////////////////
void CTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(graphics.getRenderer(), mTexture, clip, &renderQuad, angle, center, flip);
}
//////////////////////////////////////////////////////////
int CTexture::getWidth()
{
	return mWidth;
}
//////////////////////////////////////////////////////////
int CTexture::getHeight()
{
	return mHeight;
}
//////////////////////////////////////////////////////////
bool CTexture::loadFromRenderedText(TTF_Font* gFont, std::string textureText, SDL_Color textColor)
{
	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(graphics.getRenderer(), textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}
	}

	//Return success
	return mTexture != NULL;
}
//////////////////////////////////////////////////////////
void CTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}
//////////////////////////////////////////////////////////
void CTexture::setBlendMode(SDL_BlendMode blending)
{
	//Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}
//////////////////////////////////////////////////////////
void CTexture::setAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}