#pragma once

#include <SDL.h>

#include <vector>
#include "Vector2.h"
#include <string>

class GameObject;
class Text;
class QuadTree;

class Level
{
	Text* m_text;
	std::vector<GameObject*> m_objects;
	SDL_Rect m_camera;
	QuadTree* m_tree;

public:
	Level(std::string levelName);
	void LoadObjectsFromJSON(std::string levelName);
	~Level();
	void Update(float deltaTime);
	void Render();

	std::vector<GameObject*> GetObjects() { return m_objects; }
	GameObject* CreateObject(int type);
};