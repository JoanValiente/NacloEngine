#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <vector>
#include "Globals.h"

struct GameObject;

class Component {

public: 
	enum COMPONENT_TYPE {
		COMPONENT_NONE,
		COMPONENT_TRANSFORM,
		COMPONENT_MESH,
		COMPONENT_MATERIAL,
		COMPONENT_CAMERA
	};

public:
	Component(GameObject* container);
	~Component();

	virtual void Update(float dt);
	void CleanUp();

public:
	bool active = false;
	GameObject* container;
	COMPONENT_TYPE type;
};

#endif
