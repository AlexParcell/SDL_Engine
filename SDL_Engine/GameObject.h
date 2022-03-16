#pragma once
#include <SDL.h>
#include <SDL_Mixer.h>
#include "Vector2.h"
#include <string>

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
	Obj_Fridge,
	Obj_Cabinets,
	Obj_Oven,
	Tile_Wall,
	Obj_WelcomeMat,
	Obj_Cat,
	NumObjects
};

static std::string s_gameObjectType[NumObjects] = {
	"Player",
	"Wood",
	"WoodDark",
	"WallLower",
	"WallHigher",
	"Ceiling",
	"Blocker",
	"Fridge",
	"Cabinets",
	"Oven",
	"Wall",
	"WelcomeMat",
	"Cat"
};

static int objs_stringToEnum(std::string s)
{
	for (int i = 0; i < NumObjects; i++)
	{
		if (s == s_gameObjectType[i])
			return i;
	}
}

class Sprite;

class GameObject
{
protected:
	Sprite* m_sprite;

	Vector2 m_velocity;
	Vector2 m_size;

	Vector2 m_spriteOffset;
	Vector2 m_spriteSize;

public:
	Vector2 m_position;
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