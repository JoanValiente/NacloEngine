#ifndef _MODULE_LOADMESHES_
#define _MODULE_LOADMESHES_


#include "Module.h"
#include "OpenGL.h"
#pragma comment (lib,"Assimp/libx86/assimp.lib")

class ModuleLoadMeshes : public Module
{
public:
	ModuleLoadMeshes(Application* app, bool start_enabled = true);
	~ModuleLoadMeshes();
	bool Start();
	bool CleanUp();

	void LoadFBX(const char* path);

public:
	GLuint id_indices = 0;
	uint num_indices = 0;
	uint* indices = nullptr;

	GLuint id_vertices = 0;
	uint num_vertices = 0;
	float* vertices = nullptr;

};

#endif // !_MODULE_LOADMESHES_