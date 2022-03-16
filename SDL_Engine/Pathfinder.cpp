#include "Pathfinder.h"
#include "GameObject.h"
#include "LevelHandler.h"
#include "Level.h"

#include <iostream>
#include <queue>

std::vector<std::vector<GameObject*>> Pathfinder::m_tilemap = std::vector<std::vector<GameObject*>>();

std::vector<Vector2> Pathfinder::GetPath(Vector2 begin, Vector2 end)
{
	// Out of bounds, we have no knowledge of the land the light does not touch
	if (begin.x < 0 || end.x < 0 || begin.x > MAP_SIZE_X + TILE_SIZE || end.x > MAP_SIZE_X + TILE_SIZE
		|| begin.y < 0 || end.y < 0 || begin.y > MAP_SIZE_Y + TILE_SIZE || end.y > MAP_SIZE_Y + TILE_SIZE)
		return std::vector<Vector2>();

	// JIT tilemap build just to save us the perf if we never use pathfinding for some reason
	if (m_tilemap.size() == 0)
		BuildTilemap();

	// A cache to keep track of all our nodes during the run
	// Keeping it internal to this so we can have parallel path calculation
	std::vector<Node*> cache;
	auto GetNode = [&](const GameObject* obj)
	{
		for (Node* n : cache)
		{
			if (n->m_obj == obj)
				return n;
		}

		Node* newNode = new Node((GameObject*)obj);
		cache.push_back(newNode);
		return newNode;
	};

	// Okay now let's do it
	Node* startNode = GetNode(GetObjectAtPosition(begin));
	Node* endNode = GetNode(GetObjectAtPosition(end));
	if (startNode == endNode)
		return std::vector<Vector2>();

	// Priority queue so we always get the one with the lowest fCost when we pop
	auto cmp = [](Node* left, Node* right) { return left->m_fCost > right->m_fCost; };
	std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)> openList(cmp);

	startNode->m_parent = nullptr;
	startNode->DeriveHeuristics(startNode, endNode);
	startNode->m_open = true;
	openList.push(startNode);

	Node* cursor = startNode;
	while (endNode->m_parent == nullptr)
	{
		std::vector<GameObject*> adjacentObjects = GetAdjacentTiles(cursor->m_obj);
		for (GameObject* obj : adjacentObjects)
		{
			Node* node = GetNode(obj);
			if (!node->m_open && !node->m_closed)
			{
				node->m_open = true;
				node->m_parent = cursor;
				node->DeriveHeuristics(startNode, endNode);
				openList.push(node);
			}
		}

		cursor->m_closed = true;

		if (cursor->m_obj != endNode->m_obj)
		{
			cursor = openList.top();
			openList.pop();
		}
	}

	std::vector<Vector2> path;

	cursor = endNode;
	while (cursor != nullptr)
	{
		path.insert(path.begin(), cursor->m_obj->m_position);
		cursor = cursor->m_parent;
	}

	for (Node* n : cache)
		delete n;

	cache.clear();

	return path;
}

void Pathfinder::BuildTilemap()
{
	m_tilemap.resize(MAP_SIZE_X / TILE_SIZE);
	for (std::vector<GameObject*>& vec : m_tilemap)
	{
		vec.resize(MAP_SIZE_Y / TILE_SIZE);
		for (int i = 0; i < vec.size(); i++)
			vec[i] = nullptr;
	}

	for (int x = 0; x < MAP_SIZE_X; x += TILE_SIZE)
	{
		for (int y = 0; y < MAP_SIZE_Y; y += TILE_SIZE)
		{
			std::vector<GameObject*> objs = LevelHandler::GetActiveLevel()->GetObjects();
			for (GameObject* obj : objs)
			{
				// Skip stuff that moves, we don't want that smoke
				if (obj->m_type == Obj_Cat || obj->m_type == Obj_Player)
					continue;

				if (obj->GetPosition().x == (float)x && obj->GetPosition().y == (float)y)
					m_tilemap[((int)x)/TILE_SIZE][((int)y)/TILE_SIZE] = obj;
			}
		}
	}

	std::cout << "Tilemap built." << std::endl;
}

// Relies on the tilemap being built
std::vector<GameObject*> Pathfinder::GetAdjacentTiles(GameObject* tile)
{
	std::vector<GameObject*> adjacents;

	int centre_x = (int)tile->m_position.x / TILE_SIZE;
	int centre_y = (int)tile->m_position.y / TILE_SIZE;

	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			// Ignore self
			if (x == 0 && y == 0)
				continue;

			// Ignore diagonals
			if (x != 0 && y != 0)
				continue;

			int adjusted_x = centre_x + x;
			int adjusted_y = centre_y + y;
			
			if (adjusted_x < 0 || adjusted_x >= (MAP_SIZE_X / TILE_SIZE))
				continue;

			if (adjusted_y < 0 || adjusted_y >= (MAP_SIZE_Y / TILE_SIZE))
				continue;

			GameObject* obj = m_tilemap[adjusted_x][adjusted_y];

			// Ignore blocked tiles
			if (obj->m_collisionType == CT_Block)
				continue;

			adjacents.push_back(obj);
		}
	}

	return adjacents;
}

GameObject* Pathfinder::GetObjectAtPosition(Vector2 pos)
{
	return m_tilemap[(int)pos.x / TILE_SIZE][(int)pos.y / TILE_SIZE];
}

void Pathfinder::Node::DeriveHeuristics(Node* start, Node* end)
{
	float g_cost = 0;
	Node* cursor = this;

	// G Cost: Vertices from start
	while (cursor->m_parent != nullptr)
	{
		cursor = cursor->m_parent;
		g_cost++;
	}

	// H Cost: euclidean distance from the end
	float h_cost = (start->m_obj->m_position - end->m_obj->m_position).magnitude();

	m_fCost = h_cost + g_cost;
}
