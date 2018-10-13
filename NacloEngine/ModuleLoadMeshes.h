#ifndef _MODULE_LOADMESHES_
#define _MODULE_LOADMESHES_


#include "Module.h"
#include "OpenGL.h"
#include "Assimp/include/cimport.h"
#include "Devil/include/ilut.h"
#include "Devil/include/il.h"
#include "MathGeoLib/MathGeoLib.h"


struct Mesh
{
	std::string path = "";
	std::string filename = "";

	float3 position = { 0,0,0 };
	float3 rotation = { 0,0,0 };
	float3 scale = { 1,1,1 };

	uint id_indices = 0;
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_vertices = 0;
	uint num_vertices = 0;
	float* vertices = nullptr;

	uint id_texture = 0;
	uint num_texture = 0;
	float* texture = nullptr;

	uint texture_path = 0;

	uint id_color = 0;
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
	uint LoadTexture(const char* path);

	void ShowMeshInformation(const Mesh* mesh);

	const float3 GetFbxPosition(const Mesh* mesh);
	const float3 GetFbxScale(const Mesh* mesh);
	const float3 GetFbxRotation(const Mesh* mesh);

public:
	Mesh mesh;
};

#endif // !_MODULE_LOADMESHES_