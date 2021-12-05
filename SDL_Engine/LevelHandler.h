#pragma once

#include <SDL.h>
#include <string>

class Level;

class LevelHandler
{
	static Level* m_currentLevel;

public:
	static void OpenLevel(std::string levelName);
	static void Update(float deltaTime);
	static void Render();

	static Level* GetActiveLevel() { return m_currentLevel; }
	static void Free();
};