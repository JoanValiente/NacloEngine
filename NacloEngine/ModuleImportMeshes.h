#ifndef _MODULE_IMPORTMESHES_
#define _MODULE_IMPORTMESHES_


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

class ModuleImportMeshes : public Module
{
public:
	ModuleImportMeshes(Application* app, bool start_enabled = true);
	~ModuleImportMeshes();
	bool Init();
	bool Start();
	bool CleanUp();

	void LoadChildren(const aiScene* scene, aiNode* node, const char* path, GameObject* obj);

	void SetBuffers(Mesh* mesh);
	void ExportNCL(const void* buffer, Mesh* mesh);
	Mesh* ImportNCL(const char* path);
	Mesh* LoadNCL(const void* buffer);

	virtual bool Import(const char* path, std::string& output_file);
	virtual bool Import(const void* buffer, uint size, std::string& output_file, const char* path);

	void ShowMeshInformation();

	const float3 GetFbxPosition(const Mesh* mesh);
	const float3 GetFbxScale(const Mesh* mesh);
	const Quat GetFbxRotation(const Mesh* mesh);
};

#endif // !_MODULE_IMPORTMESHES_