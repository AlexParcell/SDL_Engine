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
	Tile_Wood,
	Tile_WoodDark,
	Tile_WallLower,
	Tile_WallHigher,
	Tile_Ceiling,
	Misc_Blocker,
	NumObjects
};

class Sprite;

class GameObject
{
protected:
	Sprite* m_sprite;

	Vector2 m_velocity;

	Vector2 m_position;
	Vector2 m_size;

	Vector2 m_spriteOffset;
	Vector2 m_spriteSize;

public:
	bool m_moved;
	int m_zIndex;
	int m_collisionType;
	int m_type;

	GameObject(int type);
	~GameObject();

	Vector2 GetOrigin();
	virtual SDL_Rect GetBoundingBox();

	bool TestAxis(Vector2 axis, float min_a, float min_b, float max_a, float max_b, Vector2& mtvAxis, float& mtvDistance);

	void SetPosition(Vector2 pos) { m_position = pos; }
	Vector2 GetPosition() { return m_position; }

	void SetSize(Vector2 size) { m_size = size; }
	Vector2 GetSize() { return m_size; }

	virtual void OnOverlap(GameObject* other);
	virtual void Update(float deltaTime);
	virtual void Render();
	virtual void OnKeyDown(SDL_Keycode key);
	virtual void OnKeyUp(SDL_Keycode key);
};