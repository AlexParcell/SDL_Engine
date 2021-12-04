#include "Sprite.h"
#include <iostream>

#include <SDL_image.h>
#include "Vector2.h"

extern SDL_Renderer* g_renderer;

Sprite::Sprite(std::string filepath)
	: m_texture(nullptr)
{
	SDL_Surface* surface = IMG_Load(filepath.c_str());
	if (surface != NULL)
	{
		m_texture = SDL_CreateTextureFromSurface(g_renderer, surface);
		SDL_FreeSurface(surface);
	}
}

Sprite::~Sprite()
{
	SDL_DestroyTexture(m_texture);
}

void Sprite::Render()
{
	SDL_RenderCopy(g_renderer, m_texture, NULL, NULL);
}

void Sprite::Render(SDL_Rect* source_rect, SDL_Rect* dest_rect)
{
	SDL_RenderCopy(g_renderer, m_texture, source_rect, dest_rect);
}

void Sprite::Render(Vector2& pos, Vector2& size, Vector2& offset, Vector2& spritesize)
{
	SDL_Rect sourceRect;
	sourceRect.x = (int)offset.x;
	sourceRect.y = (int)offset.y;
	sourceRect.w = (int)spritesize.x;
	sourceRect.h = (int)spritesize.y;

	SDL_Rect destRect;
	destRect.x = (int)pos.x;
	destRect.y = (int)pos.y;
	destRect.w = (int)size.x;
	destRect.h = (int)size.y;

	Render(&sourceRect, &destRect);
}

