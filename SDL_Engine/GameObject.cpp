#include "GameObject.h"
#include "Sprite.h"
#include "AudioHandler.h"

#include <math.h>

#include <iostream>

GameObject::GameObject(int type)
	: m_sprite(nullptr),
	m_position(0, 0),
	m_size(48, 48),
	m_spriteOffset(0, 0),
	m_spriteSize(24, 24),
	m_moveLeft(false),
	m_moveRight(false),
	m_moveUp(false),
	m_moveDown(false),
	m_moved(false),
	m_playerControl(false),
	m_collisionType(CT_Overlap)
{
	if (type == Obj_Player)
	{
		m_playerControl = true;
		m_position = Vector2(50, 50);
		m_collisionType = CT_Block;
	}
	else if (type == Obj_Rock)
	{
		m_collisionType = CT_Block;
	}

	m_sprite = new Sprite("pacman.png");
}

GameObject::~GameObject()
{
	delete m_sprite;
}

Vector2 GameObject::GetOrigin()
{
	return m_position + (m_size / 2);
}

SDL_Rect GameObject::GetBoundingBox()
{
	SDL_Rect box;
	box.x = m_position.x;
	box.y = m_position.y;
	box.w = m_size.x;
	box.h = m_size.y;
	return box;
}

bool GameObject::TestAxis(Vector2 axis, float min_a, float min_b, float max_a, float max_b, Vector2& mtvAxis, float& mtvDistance)
{
	float axisLengthSquared = axis.dotProduct(axis);

	float d0 = (max_b - min_a);
	float d1 = (max_a - min_b);

	if (d0 <= 0.0f || d1 <= 0.0)
		return false;

	float overlap = (d0 < d1) ? d0 : -d1;

	Vector2 sep = axis * (overlap / axisLengthSquared);
	float sepLengthSquared = sep.dotProduct(sep);

	if (sepLengthSquared < mtvDistance)
	{
		mtvDistance = sepLengthSquared;
		mtvAxis = sep;
	}

	return true;
}

void GameObject::OnOverlap(GameObject* other)
{
	// If we block, and so they, adjust our position to resolve the collision
	if (m_collisionType == CT_Block && other->m_collisionType == CT_Block)
	{
		float mtvDistance = FLT_MAX;
		Vector2 mtvAxis;

		TestAxis(Vector2(1, 0), m_position.x, other->m_position.x, m_position.x + m_size.x, other->m_position.x + other->m_size.x, mtvAxis, mtvDistance);
		TestAxis(Vector2(0, 1), m_position.y, other->m_position.y, m_position.y + m_size.y, other->m_position.y + other->m_size.y, mtvAxis, mtvDistance);

		Vector2 penetrationVector = mtvAxis.normalize();
		float penetrationDepth = sqrt(mtvDistance) * 1.001f;

		m_position += (penetrationVector * penetrationDepth);
	}
}

void GameObject::Update(float deltaTime)
{
	m_velocity = Vector2(0, 0);
	Vector2 direction;
	if (m_moveUp)
	{
		direction.y = -1;
	}
	else if (m_moveDown)
	{
		direction.y = 1;
	}

	if (m_moveLeft)
	{
		direction.x = -1;
	}
	else if (m_moveRight)
	{
		direction.x = 1;
	}

	m_velocity = direction * deltaTime * 100;
	if (m_velocity.magnitudeSquared() > 0)
		m_moved = true;
	else
		m_moved = false;

	m_position += m_velocity;
}

void GameObject::Render()
{
	if (m_sprite)
		m_sprite->Render(m_position, m_size, m_spriteOffset, m_spriteSize);
}

void GameObject::OnKeyDown(SDL_Keycode key)
{
	if (!m_playerControl) return;

	if (key == SDLK_1)
		AudioHandler::PlaySoundEffect(SFX_Low);

	if (key == SDLK_2)
		AudioHandler::PlaySoundEffect(SFX_Medium);

	if (key == SDLK_3)
		AudioHandler::PlaySoundEffect(SFX_High);

	if (key == SDLK_4)
		AudioHandler::PlaySoundEffect(SFX_Scratch);

	if (key == SDLK_5)
	{
		if (Mix_PlayingMusic() == 0)
		{
			AudioHandler::PlaySong(SONG_Beat);
		}
		else
		{
			AudioHandler::ToggleSong();
		}
	}

	if (key == SDLK_6)
	{
		AudioHandler::StopSong();
	}

	if (key == SDLK_w)
	{
		m_moveUp = true;
	}
	if (key == SDLK_a)
	{
		m_moveLeft = true;
	}
	if (key == SDLK_s)
	{
		m_moveDown = true;
	}
	if (key == SDLK_d)
	{
		m_moveRight = true;
	}
}

void GameObject::OnKeyUp(SDL_Keycode key)
{
	if (!m_playerControl) return;

	if (key == SDLK_w)
	{
		m_moveUp = false;
	}
	if (key == SDLK_a)
	{
		m_moveLeft = false;
	}
	if (key == SDLK_s)
	{
		m_moveDown = false;
	}
	if (key == SDLK_d)
	{
		m_moveRight = false;
	}
}