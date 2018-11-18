#ifndef _MESHIMPORTER_
#define _MESHIMPORTER_


#include "Importer.h"
#include "OpenGL.h"
#include "Assimp/include/cimport.h"
#include "Devil/include/ilut.h"
#include "Devil/include/il.h"
#include "MathGeoLib/MathGeoLib.h"

struct Mesh;
struct Texture;
struct GameObject;
struct aiScene;
struct aiNode;
struct aiMaterial;

class MeshImporter : public Importer
{
public:
	MeshImporter();
	~MeshImporter();
	bool Init();
	bool Start();
	bool CleanUp();

	//Load Own Format
	Mesh* ImportNCL(const char* path);
	
	//Import FBX - NCL
	bool Import(const char* path, std::string& output_file);

private:
	Mesh * LoadNCL(const void* buffer, uint size);

	bool Import(const void* buffer, uint size, std::string& output_file, const char* path);
	void ExportNCL(const void* buffer, Mesh* mesh, std::string& output_file);

	Texture* GetTexture(aiMaterial* tex, const char* path);
	
	void LoadMeshData(const aiScene* scene, aiNode* node, const char* path, GameObject* obj);
	void LoadMeshNCL(const char* path, Mesh* mesh);

	void SetBuffers(Mesh* mesh);

};

#endif // !_MESHIMPORTER_