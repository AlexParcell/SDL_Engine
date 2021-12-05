#include "GameObject.h"
#include "Sprite.h"
#include "AudioHandler.h"

#include <math.h>

#include <iostream>

GameObject::GameObject(int type)
	: m_sprite(nullptr),
	m_position(0, 0),
	m_size(32, 32),
	m_spriteOffset(0, 0),
	m_spriteSize(16, 16),
	m_moved(false),
	m_collisionType(CT_Overlap),
	m_type(type),
	m_zIndex(0)
{
	switch (type)
	{
	case(Tile_WallLower):
	{
		m_sprite = new Sprite("decoration.png");
		m_collisionType = CT_Block;
		m_spriteOffset.y = 16;
	}
	break;
	case(Tile_WallHigher):
	{
		m_sprite = new Sprite("decoration.png");
		m_collisionType = CT_Block;
	}
	break;
	case(Tile_Wood):
	{
		m_sprite = new Sprite("decoration.png");
		m_spriteOffset = Vector2(16, 16);
	}
	break;
	case(Tile_WoodDark):
	{
		m_sprite = new Sprite("decoration.png");
		m_spriteOffset.y = 32;
	}
	break;
	case(Tile_Ceiling):
	{
		m_sprite = new Sprite("decoration.png");
		m_collisionType = CT_Block;
		m_spriteOffset.x = 16;
	}
	break;
	case(Misc_Blocker):
	{
		m_collisionType = CT_Block;
	}
	break;
	}
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
		float mtvDistance = FLT_MAX;
		Vector2 mtvAxis;

		SDL_Rect box = GetBoundingBox();
		SDL_Rect otherBox = other->GetBoundingBox();

		if (!TestAxis(Vector2(1, 0), box.x, otherBox.x, box.x + box.w, otherBox.x + otherBox.w, mtvAxis, mtvDistance))	
			return;

		if (!TestAxis(Vector2(0, 1), box.y, otherBox.y, box.y + box.h, otherBox.y + otherBox.h, mtvAxis, mtvDistance))
			return;

		Vector2 penetrationVector = mtvAxis.normalize();
		float penetrationDepth = sqrt(mtvDistance) * 1.001f;
		Vector2 resolutionVector = (penetrationVector * penetrationDepth);

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
}

void GameObject::OnKeyDown(SDL_Keycode key)
{

}

void GameObject::OnKeyUp(SDL_Keycode key)
{

}