#include "Quadtree.h"
#include "Glew/include/glew.h"
#include "GameObject.h"


#define BUCKET_SIZE 4

Quadtree::Quadtree()
{
}

Quadtree::~Quadtree()
{
	Clear();
}

void Quadtree::Create(const AABB& box)
{
	root = new QuadtreeNode(box);
}

void Quadtree::Insert(GameObject * go)
{
	if (root != nullptr)
	{
		if (go->boundingBox.Intersects(root->box)) {
			root->Insert(go);
		}
	}
}

void Quadtree::Remove(GameObject * go)
{
	if (root != nullptr)
		root->Remove(go);
}

void Quadtree::DebugDraw()
{
	glBegin(GL_LINES);
	glLineWidth(2.0f);
	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);

	if (root != nullptr)
		root->DebugDraw();

	glEnd();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void Quadtree::Clear()
{
	RELEASE(root);
}

void Quadtree::CollectGameObjects(std::vector<GameObject*>& objects) const
{
	if (root != nullptr)
		root->CollectGameObjects(objects);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


QuadtreeNode::QuadtreeNode(const AABB & box)
{
	this->box = box;
	for (uint i = 0; i < 4; i++) {
		children[i] = nullptr;
	} 
}

QuadtreeNode::~QuadtreeNode()
{
	for (int i = 0; i < 4; ++i) {
		if (children[i] != nullptr) {
			RELEASE(children[i]);
		}
	}
}

bool QuadtreeNode::IsLeaf() const
{
	return children[0] == nullptr;
}

void QuadtreeNode::Insert(GameObject * go)
{
	if (IsLeaf() && objects.size() < BUCKET_SIZE) {
		objects.push_back(go);
	}
	else
	{
		if (IsLeaf())
			CreateChildren();

		objects.push_back(go);
		RedistributeChildren();
	}
}

void QuadtreeNode::Remove(GameObject * go)
{
	std::vector<GameObject*>::iterator it = std::find(objects.begin(), objects.end(), go);

	if (it != objects.end()) {
		objects.erase(it);
	}

	if (!IsLeaf())
	{
		for (int i = 0; i < 4; ++i) {
			children[i]->Remove(go);
		}
	}
}

void QuadtreeNode::CreateChildren()
{
	float3 size(box.Size());
	float3 new_size(size.x * 0.5f, size.y, size.z * 0.5f);

	float3 center(box.CenterPoint());
	float3 new_center(center);
	AABB new_box;

	/*
			 --------- MaxPoint
			 | 3 | 0 |
			 |-------|
			 | 2 | 1 |
	MinPoint ---------

	*/
	new_center.x = center.x + size.x * 0.25f;
	new_center.z = center.z + size.z * 0.25f;
	new_box.SetFromCenterAndSize(new_center, new_size);
	children[0] = new QuadtreeNode(new_box);

	new_center.x = center.x + size.x * 0.25f;
	new_center.z = center.z - size.z * 0.25f;
	new_box.SetFromCenterAndSize(new_center, new_size);
	children[1] = new QuadtreeNode(new_box);

	new_center.x = center.x - size.x * 0.25f;
	new_center.z = center.z - size.z * 0.25f;
	new_box.SetFromCenterAndSize(new_center, new_size);
	children[2] = new QuadtreeNode(new_box);

	new_center.x = center.x - size.x * 0.25f;
	new_center.z = center.z + size.z * 0.25f;
	new_box.SetFromCenterAndSize(new_center, new_size);
	children[3] = new QuadtreeNode(new_box);
}

void QuadtreeNode::RedistributeChildren()
{
	for (std::vector<GameObject*>::iterator it = objects.begin(); it != objects.end();)
	{
		GameObject* go = (*it);

		uint num_intersections = 0;
		bool intersects[4];

		for (int i = 0; i < 4; ++i) {
			intersects[i] = children[i]->box.Intersects((*it)->boundingBox);
		}

		if (!intersects[0] || !intersects[1] || !intersects[2] || !intersects[3])
		{
			it = objects.erase(it);			
			for (uint i = 0; i < 4; ++i)
			{
				if (intersects[i])
				{
					children[i]->Insert(go);
				}
			}
		}
		else
		{
			it++;
		}
	}
}

void QuadtreeNode::DebugDraw()
{
	for (uint i = 0; i < 12; i++)
	{
		glVertex3f(box.Edge(i).a.x, box.Edge(i).a.y, box.Edge(i).a.z);
		glVertex3f(box.Edge(i).b.x, box.Edge(i).b.y, box.Edge(i).b.z);
	}
	if (children[0] != nullptr)
	{
		for (uint i = 0; i < 4; ++i)
		{
			children[i]->DebugDraw();
		}
	}
}

void QuadtreeNode::CollectGameObjects(std::vector<GameObject*>& objects) const
{
	for (std::vector<GameObject*>::const_iterator it = this->objects.begin(); it != this->objects.end(); ++it) {
		objects.push_back(*it);
	}
	for (int i = 0; i < 4; ++i) {
		if (children[i] != nullptr) {
			children[i]->CollectGameObjects(objects);
		}
	}
}
