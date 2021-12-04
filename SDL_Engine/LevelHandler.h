#pragma once

#include <SDL.h>

class Level;

class LevelHandler
{
	static Level* m_currentLevel;

public:
	static void OpenLevel();
	static void Update(float deltaTime);
	static void Render();

	static Level* GetActiveLevel() { return m_currentLevel; }
	static void Free();
};