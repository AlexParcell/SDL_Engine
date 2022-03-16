#include "State.h"
#include "Pathfinder.h"
#include "Cat.h"
#include "LevelHandler.h"
#include "GameObject.h"
#include "Player.h"
#include "Level.h"
#include <iostream>

void State_FollowPlayer::Enter(Cat* cat)
{
	std::cout << "Entering default state" << std::endl;
}

void State_FollowPlayer::Exit(Cat* cat)
{
	std::cout << "Exiting default state" << std::endl;
}

void State_FollowPlayer::Update(float dt, Cat* cat)
{
	Vector2 goal = LevelHandler::GetActiveLevel()->m_player->m_position + Vector2(0, 64);

	if ((cat->m_position - goal).magnitude() < 64.0f)
	{
		cat->m_direction = Vector2(0, 0);
		m_path.clear();
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

void State_Wander::Enter(Cat* cat)
{
}

void State_Wander::Exit(Cat* cat)
{
}

void State_Wander::Update(float dt, Cat* cat)
{
	const float interval = 5.0f;
	m_moveTimer += dt;
	if (m_moveTimer > interval)
	{
		int axis = rand() % 2;
		if (axis == 0)
		{
			int direction = rand() % 2;
			if (direction == 0)
			{
				cat->m_direction = Vector2(1, 0);
			}
			else
			{
				cat->m_direction = Vector2(-1, 0);
			}
		}
		else
		{
			int direction = rand() % 2;
			if (direction == 0)
			{
				cat->m_direction = Vector2(0, 1);
			}
			else
			{
				cat->m_direction = Vector2(0, -1);
			}
		}
		m_moveTimer = 0.0f;
	}
}
