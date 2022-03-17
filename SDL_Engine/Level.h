#pragma once

#include <SDL.h>

#include <vector>
#include "Vector2.h"
#include <string>

class GameObject;
class Cat;
class Text;
class Player;
class QuadTree;

class Level
{
	Text* m_text;
	std::vector<GameObject*> m_objects;
	std::vector<Cat*> m_cats;
	SDL_Rect m_camera;
	QuadTree* m_tree;

public:
	Level(std::string levelName);
	void LoadObjectsFromJSON(std::string levelName);
	~Level();
	void Update(float deltaTime);
	void Render();

	std::vector<GameObject*> GetObjects() { return m_objects; }
	std::vector<Cat*> GetCats() { return m_cats; }
	GameObject* CreateObject(int type);
	std::vector<Cat*> GetAllCatsWithinRadius(Vector2 point, float distance);
};