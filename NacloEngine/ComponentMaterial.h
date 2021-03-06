#ifndef __COMPONENTMATERIAL_H__
#define __COMPONENTMATERIAL_H__

#include "Component.h"
#include "Globals.h"

struct Texture;

class ComponentMaterial : public Component {

public:
	ComponentMaterial(GameObject* container);
	~ComponentMaterial();

	void AssignTexture(Texture* texture);
	void ShowInspector();
	void SaveComponent(Config &conf);
	void LoadComponent(Config &conf);

public:
	Texture * texture = nullptr;
};

#endif
