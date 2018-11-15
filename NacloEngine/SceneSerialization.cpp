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

	file = new Config(file_name);

	for (std::vector<GameObject*>::const_iterator it = App->scene->gameObjects.begin(); it != App->scene->gameObjects.end(); ++it)
	{
		(*it)->SaveGO(file);
	}

	file->Save();

	return ret;
}

bool SceneSerialization::LoadScene(const char * file_name)
{
	bool ret = true;
	return ret;
}
