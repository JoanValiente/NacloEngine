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
struct Config;

class GameObject {
	 
public:
	GameObject(GameObject* parent, const char* name);
	~GameObject();
	   	 
	void Update(float dt);
	void CleanUp();

	Component* NewComponent(Component::COMPONENT_TYPE type);
	void DeleteComponent(Component* component);

	Component* GetComponentByType(Component::COMPONENT_TYPE type);	

	//void CreateBoundingBox(Mesh* mesh);
	void UpdateBoundingBox();
	void BoundingBoxDebugDraw();
	
	void ChangeHierarchy(GameObject* &obj);

	uint GetNumChildren();

	void Inspector();

	bool SaveGO(Config* &conf);

private:
	bool SearchForParent(GameObject* parent, GameObject* child);
	void EqualGameObjects(GameObject* copy, GameObject* &paste);

public:
	UID UID = 0;
	AABB boundingBox;
	bool active = false;
	bool staticGO = false;
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

