#ifndef _MODULE_LOADMESHES_
#define _MODULE_LOADMESHES_


#include "Module.h"
#include "OpenGL.h"
#include "Assimp/include/cimport.h"
#include "Devil/include/ilut.h"
#include "Devil/include/il.h"
#include "MathGeoLib/MathGeoLib.h"

struct Mesh;
struct GameObject;
struct aiScene;
struct aiNode;

class ModuleLoadMeshes : public Module
{
public:
	ModuleLoadMeshes(Application* app, bool start_enabled = true);
	~ModuleLoadMeshes();
	bool Init();
	bool Start();
	bool CleanUp();

	void LoadFBX(const char* path);

	void LoadChildren(const aiScene* scene, aiNode* node, const char* path, GameObject* obj);
	void SetBuffers(Mesh* mesh);

	bool Import(const char* path, Mesh* mesh);

	void ShowMeshInformation();

	const float3 GetFbxPosition(const Mesh* mesh);
	const float3 GetFbxScale(const Mesh* mesh);
	const Quat GetFbxRotation(const Mesh* mesh);
};

#endif // !_MODULE_LOADMESHES_