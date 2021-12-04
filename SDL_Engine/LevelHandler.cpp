#include "LevelHandler.h"
#include "Level.h"

Level* LevelHandler::m_currentLevel = nullptr;

void LevelHandler::OpenLevel()
{
	if (m_currentLevel)
		delete m_currentLevel;

	m_currentLevel = new Level();
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
