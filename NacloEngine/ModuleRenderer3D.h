#ifndef __ModuleRenderer3D_H__
#define __ModuleRenderer3D_H__

#include "Module.h"
#include "Light.h"
#include "Globals.h"
#include "ModuleLoadMeshes.h"


#include <list>

#define MAX_LIGHTS 8

struct Mesh
{
	std::string path = "";
	std::string filename = "";

	float3 position = { 0,0,0 };
	Quat rotation = Quat::identity;
	float3 scale = { 1,1,1 };

	uint id_indices = 0;
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_vertices = 0;
	uint num_vertices = 0;
	float* vertices = nullptr;

	uint id_color = 0;
	uint num_color = 0;
	float* colors = nullptr;
	aiColor4D color;
};

struct Texture
{
	uint id_texture = 0;
	uint num_texture = 0;
	float* texture = nullptr;

	uint texture_path = 0;
};

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void DrawMesh(Mesh* meshes, Texture* texture);

	void OnResize(int width, int height);

	void AddMesh(Mesh* mesh);
	void AddTexture(Texture* tex);

	void GetMeshMinMaxVertices(Mesh* mesh);
	void ClearMeshes();
	void DeleteAllMeshes();

	void AddTexture(const char* path);

	void ShowRenderInfo();


private:
	float4x4 perspective(float fovy, float aspect, float n, float f);

public:
	bool wire_mode = false;
	std::vector<Mesh*> meshes;
	std::vector<Texture*> textures;
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	float3x3 NormalMatrix;
	float4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
	bool ischecked = false;
	uint checkers_path = 0;
};

#endif