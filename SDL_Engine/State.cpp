#include "State.h"
#include "Pathfinder.h"
#include "Cat.h"
#include "LevelHandler.h"
#include "GameObject.h"
#include "InputHandler.h"
#include "InputHandler.h"
#include "Player.h"
#include "Level.h"
#include <iostream>
#include "AudioHandler.h"

void State_FollowPlayer::Enter(Cat* cat)
{
}

void State_FollowPlayer::Exit(Cat* cat)
{
}

void State_FollowPlayer::Update(float dt, Cat* cat)
{
	Vector2 goal = InputHandler::GetMousePosition();
	if ((goal - cat->m_position).magnitude() < 48.0f)
	{
		cat->m_direction = Vector2(0, 0);
		return;
	}

	cat->m_direction = (goal - cat->m_position).normalize();
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

void State_Purr::Enter(Cat* cat)
{
}

void State_Purr::Exit(Cat* cat)
{
}

void State_Purr::Update(float dt, Cat* cat)
{
	cat->m_direction = Vector2(0, 0);
	if (cat->m_timeSinceLastPurr > 3.0f)
		cat->Purr();
}

void State_Hide::Enter(Cat* cat)
{
}

void State_Hide::Exit(Cat* cat)
{
}

void State_Hide::Update(float dt, Cat* cat)
{
	Cat* nearestCat = nullptr;
	float closestMag = FLT_MAX;
	for (Cat* c : LevelHandler::GetActiveLevel()->GetCats())
	{
		if (c == cat) continue;

		float mag = (c->m_position - cat->m_position).magnitude();
		if (mag < closestMag)
		{
			nearestCat = c;
			closestMag = mag;
		}
	}

	Vector2 mousePos = InputHandler::GetMousePosition();
	float distanceFromMouse = (mousePos - cat->m_position).magnitude();

	if (closestMag > 96.0f && distanceFromMouse > 96.0f)
	{
		cat->m_direction = Vector2(0, 0);
		return;
	}

	if (distanceFromMouse < closestMag || nearestCat == nullptr)
	{
		cat->m_direction = (cat->m_position - mousePos).normalize();
	}
	else
	{
		cat->m_direction = (cat->m_position - nearestCat->m_position).normalize();
	}
}

void State_Growl::Enter(Cat* cat)
{
}

void State_Growl::Exit(Cat* cat)
{
}

void State_Growl::Update(float dt, Cat* cat)
{
	if (cat->m_timeSinceLastGrowl < 3.0f)
	{
		cat->m_direction = Vector2(0, 0);
		return;
	}

	// If we have no path, look to see if there's a cat near us, if there isn't, path to one
	if (m_target == nullptr)
	{
		Cat* nearestCat = nullptr;
		float closestMag = FLT_MAX;
		for (Cat* c : LevelHandler::GetActiveLevel()->GetCats())
		{
			if (c == cat) continue;

			// We just growled at this one
			if (c == m_lastTarget) continue;

			float mag = (c->GetOrigin() - cat->GetOrigin()).magnitude();
			if (mag < closestMag)
			{
				nearestCat = c;
				closestMag = mag;
			}
		}

		m_target = nearestCat;
	}
	
	float distanceToTarget = (m_target->GetOrigin() - cat->GetOrigin()).magnitude();
	if (distanceToTarget < 48.0f)
	{
		cat->Growl();
		m_lastTarget = m_target;
		m_target = nullptr;
	}
	else
	{
		cat->m_direction = (m_target->GetOrigin() - cat->GetOrigin()).normalize();
	}
}
