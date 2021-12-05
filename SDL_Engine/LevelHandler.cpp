#include "LevelHandler.h"
#include "Level.h"
#include <string>

Level* LevelHandler::m_currentLevel = nullptr;

void LevelHandler::OpenLevel(std::string levelName)
{
	if (m_currentLevel)
		delete m_currentLevel;

	m_currentLevel = new Level(levelName);
}

void LevelHandler::Update(float deltaTime)
{
	if (m_currentLevel)
		m_currentLevel->Update(deltaTime);
}

void LevelHandler::Render()
{
	if (m_currentLevel)
		m_currentLevel->Render();
}

void LevelHandler::Free()
{
	delete m_currentLevel;
}
