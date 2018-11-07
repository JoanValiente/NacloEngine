#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <vector>
#include "Globals.h"
#include "MathGeoLib/MathGeoLib.h"
#include "Component.h"
#include "ModuleImGui.h"

struct Mesh;
struct ComponentCamera;
struct ComponentMaterial;
struct ComponentMesh;
struct ComponentTransform;

class GameObject {
	 
public:
	GameObject(GameObject* parent, const char* name);
	~GameObject();
	   	 
	void Update(float dt);
	void CleanUp();

	Component* NewComponent(Component::COMPONENT_TYPE type);
	void DeleteComponent(Component* component);

	Component* GetComponentByType(Component::COMPONENT_TYPE type);	

	void CreateBoundingBox(Mesh* mesh);
	void UpdateBoundingBox();
	void BoundingBoxDebugDraw();

	void Inspector();

public:
	AABB boundingBox;
	bool active = false;
	std::string name;
	GameObject* parent = nullptr;
	std::vector<Component*> components;
	std::vector<GameObject*> children;

	ComponentTransform* transform = nullptr;
	ComponentMaterial* material = nullptr;
	ComponentMesh* mesh = nullptr;
	ComponentCamera* camera = nullptr;
};

#endif

