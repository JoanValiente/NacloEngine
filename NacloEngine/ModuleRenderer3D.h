#ifndef __ModuleRenderer3D_H__
#define __ModuleRenderer3D_H__

#include "Module.h"
#include "Light.h"
#include "Globals.h"
#include "ModuleLoadMeshes.h"
//#include "MathGeoLib/MathGeoLib.h"

#include <list>

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void DrawMeshes(Mesh meshes);

	void OnResize(int width, int height);

	void ModuleRenderer3D::AddDataMesh(Mesh* data_mesh);

	void AddTexture(const char* path);

private:
	float4x4 perspective(float fovy, float aspect, float n, float f);

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	float3x3 NormalMatrix;
	float4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
};

#endif