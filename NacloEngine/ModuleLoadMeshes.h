#ifndef _MODULE_LOADMESHES_
#define _MODULE_LOADMESHES_


#include "Module.h"
#include "OpenGL.h"
#include "Assimp/include/cimport.h"
#include "Devil/include/ilut.h"
#include "Devil/include/il.h"
#include "MathGeoLib/MathGeoLib.h"

struct Mesh;

class ModuleLoadMeshes : public Module
{
public:
	ModuleLoadMeshes(Application* app, bool start_enabled = true);
	~ModuleLoadMeshes();
	bool Init();
	bool Start();
	bool CleanUp();

	void LoadFBX(const char* path);

	void ShowMeshInformation();

	const float3 GetFbxPosition(const Mesh* mesh);
	const float3 GetFbxScale(const Mesh* mesh);
	const float3 GetFbxRotation(const Mesh* mesh);

public:
};

#endif // !_MODULE_LOADMESHES_