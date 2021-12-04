#include "Text.h"

extern SDL_Renderer* g_renderer;

Text::Text(std::string text, SDL_Color colour)
	: m_texture(nullptr),
	m_font(TTF_OpenFont("lazy.ttf", 128))
{
	SDL_Surface* surface = TTF_RenderText_Solid(m_font, text.c_str(), colour);
	m_texture = SDL_CreateTextureFromSurface(g_renderer, surface);
	SDL_FreeSurface(surface);
	m_size = Vector2(128, 48);
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

