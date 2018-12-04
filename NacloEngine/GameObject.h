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
struct ComponentCanvas;
struct ComponentRectTransform;
struct ComponentImage;
struct Config;


class GameObject {
	 
public:
	GameObject(GameObject* parent, const char* name);
	~GameObject();
	   	 
	void Update(float dt);
	void CleanUp();

	Component* NewComponent(Component::COMPONENT_TYPE type);
	void DeleteComponent(Component* component);
	void DeleteAllComponents();

	void DeleteChildren(GameObject* go);
	void DeleteAllChildren(GameObject* go);

	Component* GetComponentByType(Component::COMPONENT_TYPE type);	

	//void CreateBoundingBox(Mesh* mesh);
	void UpdateBoundingBox();
	void BoundingBoxDebugDraw();
	
	void ChangeHierarchy(GameObject* &obj);
	void ChangeStaticChildren(GameObject* go, bool isStatic);

	uint GetNumChildren();
	void Inspector();

	void AddChildren(GameObject* children);

	bool SaveGO(Config* &conf);
	bool LoadGO(Config &conf);

	void SetActive(GameObject* go, bool state);


private:
	bool SearchForParent(GameObject* parent, GameObject* child);
	void EqualGameObjects(GameObject* copy, GameObject* &paste);

public:
	UID parent_UID = 0;
	UID goUID = 0;
	AABB boundingBox;
	bool active = false;
	bool activated = true;
	bool staticGO = false;
	std::string name;
	GameObject* parent = nullptr;
	std::vector<Component*> components;
	std::vector<GameObject*> children;

	ComponentTransform* transform = nullptr;
	ComponentMaterial* material = nullptr;
	ComponentMesh* mesh = nullptr;
	ComponentCamera* camera = nullptr;
	ComponentCanvas* canvas = nullptr;
	ComponentRectTransform* rectTransform = nullptr;
	ComponentImage* image = nullptr;

	bool selected = false;
	
};

#endif

