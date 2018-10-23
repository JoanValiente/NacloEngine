#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <vector>
#include "Globals.h"

class GameObject {
	 
public:
	GameObject(GameObject* parent);
	~GameObject();
	   	 
	bool Update(float dt);
	bool CleanUp();

private:
	bool active = false;
	char* name;
	GameObject* parent;
	std::vector<GameObject*> children;
};

#endif

