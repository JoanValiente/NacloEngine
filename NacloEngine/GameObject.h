#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <vector>
#include "Globals.h"

struct Component;

class GameObject {
	 
public:
	GameObject(GameObject* parent);
	~GameObject();
	   	 
	bool Update(float dt);
	bool CleanUp();

	void NewComponent(Component* component);
	void DeleteComponent(Component* component);

public:
	std::vector<Component*> components;
	bool active = false;
	char* name;
	GameObject* parent;
	std::vector<GameObject*> children;
};

#endif

