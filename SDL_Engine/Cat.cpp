#include "Sprite.h"
#include "AudioHandler.h"
#include <iostream>

#include <stdlib.h> 
#include <time.h> 

#include "Cat.h"

Cat::Cat(int type)
	: GameObject(type),
	m_moveUp(false),
	m_moveLeft(false),
	m_moveRight(false),
	m_moveDown(false)
{
	srand(time(NULL));
	m_collisionType = CT_Block;
	m_sprite = new Sprite("cat.png");
	m_spriteSize = Vector2(16, 16);
	m_size = Vector2(32, 32);
	m_zIndex = 32;
}

Cat::~Cat()
{

}

SDL_Rect Cat::GetBoundingBox()
{
	SDL_Rect box;
	box.x = m_position.x;
	box.y = m_position.y + (m_size.y - 4);
	box.w = m_size.x;
	box.h = 4;

	return box;
}

void Cat::OnOverlap(GameObject* other)
{
	GameObject::OnOverlap(other);

	if (other->m_collisionType == CT_Block)
	{
		if (m_moveLeft || m_moveRight)
		{
			m_moveRight = !m_moveRight;
			m_moveLeft = !m_moveLeft;
		}
		else if (m_moveUp || m_moveDown)
		{
			m_moveUp = !m_moveUp;
			m_moveLeft = !m_moveDown;
		}
	}
}

void Cat::Update(float deltaTime)
{
	GameObject::Update(deltaTime);

	static float moveTimer = 0.0f;
	const float directionInterval = 2.0f;
	moveTimer += deltaTime;

	if (moveTimer > directionInterval)
	{
		moveTimer = 0.0f;

		int direction = rand() % 8;

		m_moveLeft = false;
		m_moveRight = false;
		m_moveUp = false;
		m_moveDown = false;

		switch (direction)
		{
		case (0):
			m_moveLeft = true;
			break;
		case (1):
			m_moveRight = true;
			break;
		case (2):
			m_moveUp = true;
			break;
		case (3):
			m_moveDown = true;
			break;
		}
	}

	m_velocity = Vector2(0, 0);
	Vector2 direction = Vector2(0, 0);
	if (m_moveUp)
	{
		direction.y = -1;
		m_spriteOffset.x = 16;
	}
	else if (m_moveDown)
	{
		direction.y = 1;
		m_spriteOffset.x = 0;
	}
	else if (m_moveLeft)
	{
		direction.x = -1;
		m_spriteOffset.x = 48;
	}
	else if (m_moveRight)
	{
		direction.x = 1;
		m_spriteOffset.x = 32;
	}

	m_velocity = direction * deltaTime * 100.0f;

	if (m_velocity.magnitude() != 0)
	{
		m_moved = true;
	}
	else
	{
		m_moved = false;
	}

	static float frameTime = 0.0f;
	const float timeBetweenFrames = 0.1f;
	frameTime += deltaTime;

	if (m_moved)
	{
		if (frameTime > timeBetweenFrames)
		{
			frameTime = 0.0f;
			m_spriteOffset.y += 16;

			if (m_spriteOffset.y > 48)
				m_spriteOffset.y = 0;
		}
	}
	else
	{
		m_spriteOffset.y = 0;
	}

	m_position += m_velocity;
}

void Cat::Render()
{
	GameObject::Render();
}

void Cat::OnKeyDown(SDL_Keycode key)
{
	GameObject::OnKeyDown(key);
}

void Cat::OnKeyUp(SDL_Keycode key)
{
	GameObject::OnKeyUp(key);
}
