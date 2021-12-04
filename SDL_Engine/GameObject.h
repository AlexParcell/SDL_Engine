#pragma once
#include <SDL.h>
#include <SDL_Mixer.h>
#include "Vector2.h"

enum CollisionType
{
	CT_Overlap,
	CT_Block,
	NumCollisionTypes
};

enum GameObjectType
{
	Obj_Player,
	Obj_Rock
};

class Sprite;

class GameObject
{
	Sprite* m_sprite;

	Vector2 m_velocity;

	Vector2 m_position;
	Vector2 m_size;

	Vector2 m_spriteOffset;
	Vector2 m_spriteSize;

	bool m_playerControl;
	int m_collisionType;
	int m_type;

	bool m_moveUp;
	bool m_moveDown;
	bool m_moveLeft;
	bool m_moveRight;

public:
	bool m_moved;


	GameObject(int type);
	~GameObject();

	Vector2 GetOrigin();
	SDL_Rect GetBoundingBox();

	bool TestAxis(Vector2 axis, float min_a, float min_b, float max_a, float max_b, Vector2& mtvAxis, float& mtvDistance);

	void OnOverlap(GameObject* other);

	void Update(float deltaTime);
	void Render();
	void OnKeyDown(SDL_Keycode key);
	void OnKeyUp(SDL_Keycode key);
};