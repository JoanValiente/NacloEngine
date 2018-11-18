#ifndef __SCENESERIALIZATION_H__
#define __SCENESERIALIZATION_H__

#include <vector>

struct GameObject;
struct Config;

enum TypeSave
{
	SAVE,
	SAVE_AS,
};

class SceneSerialization
{
public:
	SceneSerialization();
	~SceneSerialization();

	bool SaveScene(const char* file_name = "");
	bool LoadScene(const char* file_name = "");

	bool ShowSavingOption(TypeSave type);
	bool ShowLoadingOption();

private:
	const char* last_saved = nullptr;

	void SetGameObjectHierarchy(GameObject* go);

	Config* file;
	bool saved_once = false;
	std::vector<GameObject*> aux_go;
};

#endif // !__SCENESERIALIZATION_H__

