#ifndef __SCENESERIALIZATION_H__
#define __SCENESERIALIZATION_H__

#include <vector>

struct GameObject;
struct Config;

class SceneSerialization
{
public:
	SceneSerialization();
	~SceneSerialization();

	bool SaveScene(const char* file_name = "");
	bool LoadScene(const char* file_name = "");


private:
	Config* file;
	std::vector<GameObject*> aux_go;
};

#endif // !__SCENESERIALIZATION_H__

