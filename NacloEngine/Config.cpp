#include "Config.h"
#include "string.h"
#include "stdio.h"

Config::Config()
{
	json_root = json_value_init_object();
	root = json_value_get_object(json_root);
}

Config::Config(const char * data)
{
	if (data != nullptr)
	{
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

size_t Config::Save(char** buffer) const
{
	size_t written = json_serialization_size(json_root);
	*buffer = new char[written];
	json_serialize_to_buffer(json_root, *buffer, written);
	return written;
}
