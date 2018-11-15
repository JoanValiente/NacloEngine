#include "SceneSerialization.h"
#include "Application.h"
#include "GameObject.h"
#include "Config.h"

SceneSerialization::SceneSerialization()
{
}

SceneSerialization::~SceneSerialization()
{
	aux_go.clear();
}

bool SceneSerialization::SaveScene(const char * file_name)
{
	bool ret = true;

	Config *file = new Config(file_name);

	file->SetArray("GAME OBJECTS");

	for (std::vector<GameObject*>::const_iterator it = App->scene->gameObjects.begin(); it != App->scene->gameObjects.end(); ++it)
	{
		(*it)->SaveGO(file);
	}

	ret = file->Save();

	delete file;
	return ret;
}

bool SceneSerialization::LoadScene(const char * file_name)
{
	bool ret = true;
	return ret;
}
