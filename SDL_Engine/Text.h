#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

#include "Vector2.h"

class Text
{
	TTF_Font* m_font;
	SDL_Texture* m_texture;

	Vector2 m_position;
	Vector2 m_size;

public:

	Text(std::string text, SDL_Color colour);
	~Text();

	void Render();
};