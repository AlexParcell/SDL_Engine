#pragma once

#include "Vector2.h"
#include "GameObject.h"
#include <vector>
#include <SDL.h>

class QuadTree
{
	Vector2 m_origin;
	Vector2 m_halfDimension;
	QuadTree* m_children[4];
	std::vector<GameObject*> m_data;
	const int m_capacity = 4;

public:

	QuadTree(const Vector2& origin, const Vector2& halfDimension)
		: m_origin(origin), m_halfDimension(halfDimension)
	{
		for (int i = 0; i < 4; i++)
			m_children[i] = nullptr;
	}

	~QuadTree()
	{
		for (int i = 0; i < 4; i++)
			delete m_children[i];
	}

	int GetQuadrantContainingPoint(const Vector2& point) const
	{
		int quad = 0;
		if (point.x >= m_origin.x) quad |= 2;
		if (point.y >= m_origin.y) quad |= 1;
		return quad;
	}

	bool IsLeafNode() const
	{
		return m_children[0] == nullptr;
	}

	void insert(GameObject* data)
	{
		if (IsLeafNode())
		{
			// If we're a leaf and we have no space, give us the data and go back
			if (m_data.size() < m_capacity)
			{
				m_data.push_back(data);
				return;
			}
			else // Else we're full, so let's subdivide
			{
				std::vector<GameObject*> oldData = m_data;
				m_data.clear();

				for (int i = 0; i < 4; i++)
				{
					Vector2 newOrigin = m_origin;
					newOrigin.x += m_halfDimension.x * (i & 2 ? .5f : -.5f);
					newOrigin.y += m_halfDimension.y * (i & 1 ? .5f : -.5f);
					m_children[i] = new QuadTree(newOrigin, m_halfDimension * 0.5f);
				}

				for (GameObject* obj : oldData)
					m_children[GetQuadrantContainingPoint(obj->GetOrigin())]->insert(obj);

				m_children[GetQuadrantContainingPoint(data->GetOrigin())]->insert(data);
			}
		}
		else
		{
			// If we're not a leaf, insert into the quadrant this is supposed to go in
			int quadrant = GetQuadrantContainingPoint(data->GetOrigin());
			m_children[quadrant]->insert(data);
		}
	}

	bool remove(GameObject* data)
	{
		if (IsLeafNode())
		{
			// If we're a leaf, null out our data

			for (int i = 0; i < m_data.size(); i++)
			{
				if (m_data[i] == data)
					m_data.erase(m_data.begin() + i);
			}

			// Indicate that we are now empty if we are
			if (m_data.size() == 0)
				return true;
			else
				return false;
		}
		else
		{
			bool makeLeaf = false;
			// Check if we're empty (true)
			for (int i = 0; i < 4; i++)
			{
				makeLeaf &= m_children[i]->remove(data);
			}

			// If we're empty, delete our children
			if (makeLeaf)
			{
				for (int i = 0; i < 4; i++)
				{
					delete m_children[i];
				}
				return true;
			}
			else
			{
				// Otherwise, return false
				return false;
			}
		}
	}

	static bool AABB(SDL_Rect a, SDL_Rect b)
	{
		if (a.x < b.x + b.w &&
			a.x + a.w > b.x &&
			a.y < b.y + b.h &&
			a.h + a.y > b.y)
			return true;
		
		return false;
	}

	void GetCollisions(GameObject* a, std::vector<GameObject*>& results)
	{
		if (IsLeafNode())
		{
			// Check if we're intersecting with the data we've found
			if (m_data.size() > 0)
			{
				for (GameObject* obj : m_data)
				{
					if (a != obj)
					{
						if (AABB(a->GetBoundingBox(), obj->GetBoundingBox()))
						{
							results.push_back(obj);
						}
					}
				}
			}
		}
		else
		{
			// Check if we've intersecting with any of the subdivisions
			for (int i = 0; i < 4; i++)
			{
				SDL_Rect bb;
				bb.x = m_children[i]->m_origin.x - m_children[i]->m_halfDimension.x;
				bb.y = m_children[i]->m_origin.y - m_children[i]->m_halfDimension.y;
				bb.w = m_halfDimension.x * 2;
				bb.h = m_halfDimension.y * 2;

				// We are, so look at the subdivision
				if (AABB(a->GetBoundingBox(), bb))
					m_children[i]->GetCollisions(a, results);
			}
		}
	}

};