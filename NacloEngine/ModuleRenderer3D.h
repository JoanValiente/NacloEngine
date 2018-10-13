#ifndef __ModuleRenderer3D_H__
#define __ModuleRenderer3D_H__

#include "Module.h"
#include "Light.h"
#include "Globals.h"
#include "ModuleLoadMeshes.h"
//#include "MathGeoLib/MathGeoLib.h"

#include <list>

#define MAX_LIGHTS 8

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

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void DrawMesh(Mesh* meshes);

	void OnResize(int width, int height);

	void AddMesh(Mesh* mesh);
	void ClearMeshes();

	void AddTexture(const char* path);

private:
	float4x4 perspective(float fovy, float aspect, float n, float f);

public:
	std::vector<Mesh*> meshes;
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	float3x3 NormalMatrix;
	float4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
};

#endif