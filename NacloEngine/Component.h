#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <vector>
#include "Globals.h"

struct GameObject;
struct Config;

class Component {

public:
	enum COMPONENT_TYPE {
		COMPONENT_NONE,
		COMPONENT_TRANSFORM,
		COMPONENT_MESH,
		COMPONENT_MATERIAL,
		COMPONENT_CAMERA,
		COMPONENT_CANVAS,
		COMPONENT_RECT_TRANSFORM,
		COMPONENT_IMAGE,
		COMPONENT_BUTTON,
		COMPONENT_LABEL
	};

public:
	Component(GameObject* container);
	virtual ~Component();

	virtual void Update(float dt);
	virtual void ShowInspector();
	virtual void SaveComponent(Config &conf);
	virtual void LoadComponent(Config &conf);

	void CleanUp();

public:
	bool active = false;
	GameObject* container;
	COMPONENT_TYPE type;

};

#endif
