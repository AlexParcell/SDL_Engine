#pragma once
#include <vector>
#include "Vector2.h"

class GameObject;

#define MAP_SIZE_X 640
#define MAP_SIZE_Y 352
#define TILE_SIZE 32

class Pathfinder
{
	struct Node
	{
	public:
		Node* m_parent;
		GameObject* m_obj;
		float m_fCost;
		bool m_closed;
		bool m_open;

		Node(GameObject* obj)
			: m_obj(obj),
			m_fCost(0.0f),
			m_closed(false),
			m_open(false),
			m_parent(nullptr)
		{

		}

		void DeriveHeuristics(Node* start, Node* end);
	};


	static std::vector<std::vector<GameObject*>> m_tilemap;
	static void BuildTilemap();
	static std::vector<GameObject*> GetAdjacentTiles(GameObject* tile);
	static GameObject* GetObjectAtPosition(Vector2 pos);

public:
	Pathfinder() {};
	~Pathfinder() {};

	static std::vector<Vector2> GetPath(Vector2 begin, Vector2 end);
};