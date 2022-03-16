#include "State.h"
#include "Pathfinder.h"
#include "Cat.h"
#include "LevelHandler.h"
#include "GameObject.h"
#include "Player.h"
#include "Level.h"
#include <iostream>

void DefaultState::Enter(Cat* cat)
{
	std::cout << "Entering default state" << std::endl;
}

void DefaultState::Exit(Cat* cat)
{
	std::cout << "Exiting default state" << std::endl;
}

void DefaultState::Update(float dt, Cat* cat)
{
	Vector2 goal = LevelHandler::GetActiveLevel()->m_player->m_position + Vector2(0, 64);

	if ((cat->m_position - goal).magnitude() < 32.0f)
	{
		cat->m_direction = Vector2(0, 0);
		return;
	}

	if (m_path.size() == 0)
	{
		m_path = Pathfinder::GetPath(cat->m_position, goal);
	}

	if (m_path.size() == 0)
	{
		cat->m_direction = Vector2(0, 0);
		return;
	}

	Vector2 point = m_path[0];
	if ((cat->m_position - point).magnitude() < 32.0f)
	{
		m_path.erase(m_path.begin());
		return;
	}
	cat->m_direction = (point - cat->m_position).normalize();
}
