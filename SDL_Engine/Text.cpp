#include "Text.h"

extern SDL_Renderer* g_renderer;

Text::Text(std::string text, SDL_Color colour, int fontsize)
	: m_texture(nullptr),
	m_fontsize(fontsize)
{
	m_font = TTF_OpenFont("lazy.ttf", fontsize);
	SDL_Surface* surface = TTF_RenderText_Solid(m_font, text.c_str(), colour);
	m_texture = SDL_CreateTextureFromSurface(g_renderer, surface);
	SDL_FreeSurface(surface);
	m_size = Vector2(text.size() * (fontsize * 0.55), fontsize);
}

Text::~Text()
{
	TTF_CloseFont(m_font);
	SDL_DestroyTexture(m_texture);
}

void Text::Render()
{
	SDL_Rect destRect;
	destRect.x = (int)m_position.x;
	destRect.y = (int)m_position.y;
	destRect.w = (int)m_size.x;
	destRect.h = (int)m_size.y;

	SDL_RenderCopy(g_renderer, m_texture, NULL, &destRect);
}

