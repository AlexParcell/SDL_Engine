#include "Level.h"
#include "GameObject.h"
#include "Text.h"
#include "Constants.h"
#include "Quadtree.h"
#include "Player.h"
#include "Cat.h"
#include "JSON.hpp"
#include <string>

#include <iostream>
#include <fstream>
#include <algorithm>

Level::Level(std::string levelName)
{
	// Load in objects from JSON for the level
	LoadObjectsFromJSON(levelName);

	// Set up the quad tree
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

std::vector<Cat*> Level::GetAllCatsWithinRadius(Vector2 point, float distance)
{
	std::vector<Cat*> cats;
	for (Cat* cat : m_cats)
	{
		if ((point - cat->m_position).magnitude() < distance)
			cats.push_back(cat);
	}
	return cats;
}

void Level::LoadObjectsFromJSON(std::string levelName)
{
	std::fstream file(levelName); // opening file
	nlohmann::json levelData; // making JSON object
	file >> levelData; // reading JSON file to object
	file.close(); // closing file

	nlohmann::json objects = levelData["LevelData"];
	for (int i = 0; i < objects.size(); i++)
	{
		std::string typeString = objects[i]["Type"];
		int type = objs_stringToEnum(typeString);

		if (type >= NumObjects)
			continue;

		Vector2 position = Vector2(objects[i]["Position"]["x"], objects[i]["Position"]["y"]);
		
		GameObject* newObject = nullptr;

		if (type == Obj_Cat)
		{
			std::string name = objects[i]["Name"];
			Personality p = Personality(
				objects[i]["Personality"]["Openness"],
				objects[i]["Personality"]["Conscientiousness"],
				objects[i]["Personality"]["Extroversion"],
				objects[i]["Personality"]["Agreeableness"],
				objects[i]["Personality"]["Neuroticism"]
			);
			newObject = new Cat(type, name, p);
			m_cats.push_back((Cat*)newObject);
		}
		else
		{
			newObject = CreateObject(type);
		}

		newObject->SetPosition(position);

		m_objects.push_back(newObject);
	}
}

Level::~Level()
{
	for (GameObject* obj : m_objects)
	{
		delete obj;
	}
	delete m_tree;
}

bool ZIndex_YPos_Sort(GameObject* a, GameObject* b)
{
	float a_value = a->GetPosition().y + a->m_zIndex;
	float b_value = b->GetPosition().y + b->m_zIndex;

	return a_value < b_value;
}

void Level::Update(float deltaTime)
{
	// Update our objects
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

	// Collisions
	for (GameObject* obj : m_objects)
	{
		std::vector<GameObject*> results;
		m_tree->GetCollisions(obj, results);
		for (GameObject* other : results)
		{
			other->OnOverlap(obj);
			obj->OnOverlap(other);
		}
	}

	// Sort for Z Indexing and Y Position
	// Sorry for this one CPU
	std::sort(m_objects.begin(), m_objects.end(), ZIndex_YPos_Sort);
}

void Level::Render()
{
	for (GameObject* obj : m_objects)
	{
		obj->Render();
	}
}

GameObject* Level::CreateObject(int type)
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

	return newObject;
}
