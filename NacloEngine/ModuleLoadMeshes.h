#ifndef _MODULE_LOADMESHES_
#define _MODULE_LOADMESHES_


#include "Module.h"
#include "OpenGL.h"
#include "Assimp/include/cimport.h"
#include "Devil/include/ilut.h"
#include "Devil/include/il.h"

struct Mesh
{
	GLuint id_indices = 0;
	uint num_indices = 0;
	uint* indices = nullptr;

	GLuint id_vertices = 0;
	uint num_vertices = 0;
	float* vertices = nullptr;

	GLuint id_texture = 0;
	uint num_texture = 0;
	float* texture = nullptr;

	GLuint id_color = 0;
	uint num_color = 0;
	float* colors = nullptr;
	aiColor4D color;
};

class ModuleLoadMeshes : public Module
{
public:
	ModuleLoadMeshes(Application* app, bool start_enabled = true);
	~ModuleLoadMeshes();
	bool Init();
	bool Start();
	bool CleanUp();

	void LoadFBX(const char* path);

public:
	Mesh mesh;
};

#endif // !_MODULE_LOADMESHES_