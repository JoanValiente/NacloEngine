#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <vector>
#include "Globals.h"

struct GameObject;

enum COMPONENT_TYPE {
	COMPONENT_NONE,
	COMPONENT_TRANSFORM,
	COMPONENT_MESH,
	COMPONENT, MATERIAL
};
class Component {

public:
	Component(GameObject* parent);
	~Component();

	virtual void Update(float dt);
	void CleanUp();

public:
	bool active = false;
	GameObject* parent;
	COMPONENT_TYPE type;
};

#endif
