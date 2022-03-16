#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

#include "Vector2.h"

class Text
{
	TTF_Font* m_font;
	SDL_Texture* m_texture;
	int m_fontsize;

public:

	Vector2 m_position;
	Vector2 m_size;

	Text(std::string text, SDL_Color colour, int fontsize);
	~Text();

	void Render();
};