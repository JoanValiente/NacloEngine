#ifndef __COMPONENTMESH_H__
#define __COMPONENTMESH_H__

#include "Component.h"
#include "Globals.h"

struct Mesh;

class ComponentMesh : public Component {

public:
	ComponentMesh(GameObject* container);
	~ComponentMesh();

	void AssignMesh(Mesh* mesh);

public:
	Mesh* mesh;
};

#endif