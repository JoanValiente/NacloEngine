#ifndef __QUADTREE_H__
#define __QUADTREE_H__

#include "Globals.h"
#include "MathGeoLib/MathGeoLib.h"
#include <vector>

struct GameObject;

class QuadtreeNode {

public:
	QuadtreeNode(const AABB& box);
	~QuadtreeNode();

	bool IsLeaf() const;

	void Insert(GameObject* go);
	void Remove(GameObject* go);
	void CreateChildren();
	void RedistributeChildren();
	void DebugDraw();

	template<typename TYPE>
	void Intersect(std::vector<GameObject*> &objects, const TYPE & primitive);
	void CollectGameObjects(std::vector<GameObject*> &objects) const;

public:
	AABB box;
	std::vector<GameObject*> objects;
	QuadtreeNode* parent = nullptr;
	QuadtreeNode* children[4];

};

class Quadtree
{
public:

	Quadtree();
	~Quadtree();

	void Create(const AABB& box);
	void Insert(GameObject* go);
	void Remove(GameObject* go);
	void DebugDraw();
	void Clear();

	template<typename TYPE>
	void Intersect(std::vector<GameObject*> &objects, const TYPE & primitive);
	void CollectGameObjects(std::vector<GameObject*> &objects) const;

public:
	QuadtreeNode* root = nullptr;
};


#endif

template<typename TYPE>
inline void Quadtree::Intersect(std::vector<GameObject*>& objects, const TYPE & primitive)
{
	if (root_node != nullptr)
		root_node->CollectIntersections(objects, primitive);
}

template<typename TYPE>
inline void QuadtreeNode::Intersect(std::vector<GameObject*>& objects, const TYPE & primitive)
{
	if (primitive.Intersects(box))
	{
		for (std::vector<GameObject*>::const_iterator it = this->objects.begin(); it != this->objects.end(); ++it)
		{
			if (primitive.Intersects((*it)->boundingBox)) {
				objects.push_back(*it);
			}
		}
		if (!isLeaf())
		{
			for (int i = 0; i < 4; ++i)
			{
				if (childrens[i] != nullptr) childrens[i]->Intersect(objects, primitive);
			}
		}
	}
}
