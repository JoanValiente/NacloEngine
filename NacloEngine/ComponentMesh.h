#ifndef __COMPONENTMESH_H__
#define __COMPONENTMESH_H__

#include "Component.h"
#include "Globals.h"
#include "ModuleRenderer3D.h"

struct Mesh;

class ComponentMesh : public Component {

public:
	ComponentMesh(GameObject* container);
	~ComponentMesh();

	void ShowInspector();
	void AssignMesh(Mesh* mesh);

	void SaveComponent(Config &conf);
	void LoadComponent(Config &conf) {};

public:
	Mesh* mesh;
};

#endif