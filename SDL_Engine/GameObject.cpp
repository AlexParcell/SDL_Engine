#include "GameObject.h"
#include "Sprite.h"
#include "AudioHandler.h"

#include <math.h>

#include <iostream>

Sprite* g_collisionBox = nullptr;

GameObject::GameObject(int type)
	: m_sprite(nullptr),
	m_position(0, 0),
	m_size(48, 48),
	m_spriteOffset(0, 0),
	m_spriteSize(24, 24),
	m_moved(false),
	m_collisionType(CT_Overlap),
	m_type(type),
	m_zIndex(0)
{
	switch (type)
	{
	case(Obj_Rock):
	{
		m_sprite = new Sprite("pacman.png");
		m_collisionType = CT_Block;
	}
	break;
	case(Tile_Grass):
	{
		m_sprite = new Sprite("grass.png");
		m_collisionType = CT_Overlap;
		m_size = Vector2(32, 32);
		m_spriteSize = Vector2(16, 16);
	}
	break;
	case(Tile_Water):
	{
		m_sprite = new Sprite("water.png");
		m_collisionType = CT_Block;
		m_size = Vector2(32, 32);
		m_spriteSize = Vector2(16, 16);
	}
	break;
	}

	g_collisionBox = new Sprite("box.png");
}

GameObject::~GameObject()
{
	if (m_sprite)
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
	if (m_collisionType == CT_Block && other->m_collisionType == CT_Block && m_moved)
	{
		std::cout << "Adjusting position" << std::endl;
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

}

void GameObject::Render()
{
	if (m_sprite)
		m_sprite->Render(m_position, m_size, m_spriteOffset, m_spriteSize);

	if (g_collisionBox)
	{
		Vector2 offset = Vector2(0, 0);
		Vector2 size = Vector2(64, 64);
		g_collisionBox->Render(m_position, m_size, offset, size);
	}
}

void GameObject::OnKeyDown(SDL_Keycode key)
{

}

void GameObject::OnKeyUp(SDL_Keycode key)
{

}