#pragma once

#include <SDL.h>
#include <stdio.h>

#include "Vector2.h"

class InputHandler
{
	static void UpdateKeyboardStates();

public:

	static void Update(float deltaTime);
	static bool IsKeyDown(SDL_Scancode key);
	static Vector2 GetMousePosition();
};