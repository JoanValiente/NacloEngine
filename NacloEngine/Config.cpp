#include "Config.h"
#include "string.h"
#include "stdio.h"

Config::Config()
{
}

Config::Config(const char * data)
{
	if (data != nullptr)
	{
		name = data;
		json_root = json_parse_file(data);

		if (json_root == nullptr)
		{
			json_root = json_value_init_object();
			root = json_value_get_object(json_root);
		}
	}
}

Config::~Config()
{
	json_value_free(json_root);
}

//---------------- BOOLEAN ----------------

bool Config::GetBool(const char * name) const
{
	return json_object_get_boolean(root, name);
}

bool Config::AddBool(const char * name, bool value)
{
	bool ret = false;
	if (json_object_set_boolean(root, name, value))
	{
		ret = true;
	}
	return ret;
}

//---------------- INT ----------------

int Config::GetInt(const char * name) const
{
	return (int)json_object_get_number(root, name);
}

bool Config::SetInt(const char * name, int value)
{
	bool ret = false;
	if (json_object_set_number(root, name, value))
	{
		ret = true;
	}
	return ret;
}

//---------------- FLOAT ----------------

int Config::GetFloat(const char * name) const
{
	return (float)json_object_get_number(root, name);
}

bool Config::SetFloat(const char * name, int value)
{
	bool ret = false;
	if (json_object_set_number(root, name, value))
	{
		ret = true;
	}
	return ret;
}

//---------------- SAVE ----------------

size_t Config::Save(char** buffer) const
{
	size_t written = json_serialization_size(json_root);
	*buffer = new char[written];
	json_serialize_to_buffer(json_root, *buffer, written);
	return written;
}
