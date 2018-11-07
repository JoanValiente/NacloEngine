#ifndef _MESHIMPORTER_
#define _MESHIMPORTER_


#include "Importer.h"
#include "OpenGL.h"
#include "Assimp/include/cimport.h"
#include "Devil/include/ilut.h"
#include "Devil/include/il.h"
#include "MathGeoLib/MathGeoLib.h"

struct Mesh;
struct GameObject;
struct aiScene;
struct aiNode;

class MeshImporter : public Importer
{
public:
	MeshImporter();
	~MeshImporter();
	bool Init();
	bool Start();
	bool CleanUp();

	void LoadMeshData(const aiScene* scene, aiNode* node, const char* path, GameObject* obj);

	void SetBuffers(Mesh* mesh);

	//Load Own Format
	Mesh* ImportNCL(const char* path);
	Mesh* LoadNCL(const void* buffer, uint size);
	
	//Import FBX - NCL
	bool Import(const char* path, std::string& output_file);
	bool Import(const void* buffer, uint size, std::string& output_file, const char* path);
	void ExportNCL(const void* buffer, Mesh* mesh);
};

#endif // !_MESHIMPORTER_