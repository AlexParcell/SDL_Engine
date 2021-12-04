#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include "InputHandler.h"
#include "LevelHandler.h"
#include "AudioHandler.h"
#include "Constants.h"

// Window to be rendering too
SDL_Window* g_window = NULL;

// Renderer
SDL_Renderer* g_renderer = NULL;

bool g_quit = false;

bool initialize()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		success = false;
	}
	else
	{
		//Create window
		g_window = SDL_CreateWindow("Alex's Really Cool Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (g_window == NULL)
		{
			success = false;
		}
		else
		{
			g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
			if (g_renderer == NULL)
			{
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

				IMG_Init(IMG_INIT_PNG);
				Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
				TTF_Init();
			}
		}
	}

	return success;
}

void exit()
{
	//Clear resources
	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);
	g_window = NULL;
	g_renderer = NULL;

	//Quit SDL subsystems
	Mix_Quit();
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

Uint64 now = SDL_GetPerformanceCounter();
Uint64 last = 0;

float GetDeltaTime()
{
	last = now;
	now = SDL_GetPerformanceCounter();
	double deltaTime = (double)(now - last) * 1000 / (double)SDL_GetPerformanceFrequency();
	deltaTime *= 0.001f; // convert to seconds
	return (float)deltaTime;
}

int main(int argc, char* args[])
{
	if (initialize())
	{
		// Start her up
		LevelHandler::OpenLevel();
		AudioHandler::Initialize();

		// Game loop
		while (!g_quit)
		{
			float deltaTime = GetDeltaTime();

			InputHandler::Update(deltaTime);
			LevelHandler::Update(deltaTime);

			SDL_RenderClear(g_renderer);

			LevelHandler::Render();

			SDL_RenderPresent(g_renderer);
		}

		// Tidy up and fuck off
		LevelHandler::Free();
		AudioHandler::Free();
	}

	exit();

	return 0;
}