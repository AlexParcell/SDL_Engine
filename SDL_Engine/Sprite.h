#pragma once

#include <SDL.h>
#include <string>

class Vector2;

class Sprite
{
	SDL_Texture* m_texture;

public:
	Sprite(std::string filepath);
	~Sprite();

	void Render();
	void Render(SDL_Rect* source_rect, SDL_Rect* dest_rect);
	void Render(Vector2& pos, Vector2& size, Vector2& offset, Vector2& spritesize);
};