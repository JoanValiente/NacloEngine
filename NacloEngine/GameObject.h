#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <vector>
#include "Globals.h"
#include "Component.h"

class GameObject {
	 
public:
	GameObject(GameObject* parent, const char* name);
	~GameObject();
	   	 
	void Update(float dt);
	void CleanUp();

	void NewComponent(Component* component);
	void DeleteComponent(Component* component);

public:
	bool active = false;
	std::string name;
	GameObject* parent = nullptr;
	std::vector<Component*> components;
	std::vector<GameObject*> children;
};

#endif

