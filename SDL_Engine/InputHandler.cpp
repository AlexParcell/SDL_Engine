#include "InputHandler.h"
#include "LevelHandler.h"
#include "Level.h"
#include "GameObject.h"
#include <vector>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_sdlrenderer.h"

extern bool g_quit;

void InputHandler::Update(float deltaTime)
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		ImGui_ImplSDL2_ProcessEvent(&e);

		switch (e.type)
		{
		case (SDL_QUIT):
			g_quit = true;
			break;
		case (SDL_KEYDOWN):
		{
			std::vector<GameObject*> objects = LevelHandler::GetActiveLevel()->GetObjects();
			for (GameObject* obj : objects)
			{
				obj->OnKeyDown(e.key.keysym.sym);
			}
		}
		break;
		case (SDL_KEYUP):
		{
			std::vector<GameObject*> objects = LevelHandler::GetActiveLevel()->GetObjects();
			for (GameObject* obj : objects)
			{
				obj->OnKeyUp(e.key.keysym.sym);
			}
		}
		break;
		}
	}
}