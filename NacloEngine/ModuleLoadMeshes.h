#ifndef _MODULE_LOADMESHES_
#define _MODULE_LOADMESHES_


#include "Module.h"
#include "OpenGL.h"
#include "Assimp/include/cimport.h"

#pragma comment (lib,"Assimp/libx86/assimp.lib")

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
	aiColor4D color_4D;
};

class ModuleLoadMeshes : public Module
{
public:
	ModuleLoadMeshes(Application* app, bool start_enabled = true);
	~ModuleLoadMeshes();
	bool Start();
	bool CleanUp();

	void LoadFBX(const char* path);

public:

	Mesh mesh;

};

#endif // !_MODULE_LOADMESHES_