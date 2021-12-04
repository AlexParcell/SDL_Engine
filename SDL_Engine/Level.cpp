#include "Level.h"
#include "GameObject.h"
#include "Text.h"
#include "Constants.h"
#include "Quadtree.h"
#include "Player.h"

#include <iostream>

Level::Level()
{
	AddObject(Obj_Player);

	Vector2 treeOrigin = Vector2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	Vector2 treeHalfDimension = Vector2(2048, 2048);
	m_tree = new QuadTree(treeOrigin, treeHalfDimension);
	for (GameObject* obj : m_objects)
		m_tree->insert(obj);

	m_camera.x = 0;
	m_camera.y = 0;
	m_camera.h = SCREEN_HEIGHT;
	m_camera.w = SCREEN_WIDTH;
}

Level::~Level()
{
	for (GameObject* obj : m_objects)
	{
		delete obj;
	}
	delete m_tree;
}

void Level::Update(float deltaTime)
{
	for (GameObject* obj : m_objects)
	{
		obj->Update(deltaTime);

		// If the object has moved, remove and reinsert into the quad tree
		if (obj->m_moved)
		{
			m_tree->remove(obj);
			m_tree->insert(obj);
		}
	}

	for (GameObject* obj : m_objects)
	{
		std::vector<GameObject*> results;
		m_tree->GetCollisions(obj, results);
		for (GameObject* other : results)
		{
			obj->OnOverlap(other);
		}
	}
}

void Level::Render()
{
	for (GameObject* obj : m_objects)
		obj->Render();
}

void Level::AddObject(int type)
{
	GameObject* newObject = nullptr;

	switch (type)
	{
	case (Obj_Player):
		newObject = new Player(type);
		break;
	default:
		newObject = new GameObject(type);
		break;
	}

	m_objects.push_back(newObject);
}
