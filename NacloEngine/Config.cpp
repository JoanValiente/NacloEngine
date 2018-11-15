#include "Config.h"
#include "string.h"
#include "stdio.h"
#include "Application.h"
#include "Parson\parson.h"

Config::Config()
{
	json_root = json_value_init_object();
	root = json_value_get_object(json_root);
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

Config::Config(JSON_Object* section) : root(section)
{
}

Config::~Config()
{
	json_value_free(json_root);
}

Config Config::GetSection(const char * section_name) const
{
	return Config(json_object_get_object(root, section_name));
}

Config Config::AddSection(const char * section_name)
{
	json_object_set_value(root, section_name, json_value_init_object());
	return GetSection(section_name);
}

//---------------- BOOLEAN ----------------

bool Config::GetBool(const char * name) const
{
	return json_object_get_boolean(root, name);
}

bool Config::SetBool(const char * name, bool value)
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

float Config::GetFloat(const char * name) const
{
	return (float)json_object_get_number(root, name);
}

bool Config::SetFloat(const char * name, float value)
{
	bool ret = false;
	if (json_object_set_number(root, name, value))
	{
		ret = true;
	}
	return ret;
}

//---------------- STRING ----------------

const char* Config::GetString(const char * name) const
{
	return json_object_get_string(root, name);
}

bool Config::SetString(const char * name, const char* value)
{
	bool ret = false;
	if (json_object_set_string(root, name, value))
	{
		ret = true;
	}
	return ret;
}


//---------------- UID ----------------

UID Config::GetUID(const char * name) const
{
	return (UID)json_object_get_number(root, name);
}

bool Config::SetUID(const char * name, UID value)
{
	bool ret = false;
	if (json_object_set_number(root, name, value))
	{
		ret = true;
	}
	return ret;
}

//---------------- ARRAY ----------------


bool Config::SetArray(const char * name)
{
	JSON_Value* new_array = json_value_init_array();
	json_array = json_value_get_array(new_array);

	return json_object_set_value(root, name, new_array);
}

bool Config::NewArrayEntry(const Config & conf)
{
	if (json_array != nullptr)
	{
		return json_array_append_value(json_array, json_value_deep_copy(conf.json_root));
	}

	else
	{
		return false;
	}
}

bool Config::SetIntArray(const char * name, int * values, int size)
{
	if (values != nullptr && size > 0)
	{
		JSON_Value* new_array = json_value_init_array();
		json_array = json_value_get_array(new_array);
		json_object_set_value(root, name, new_array);

		for (int i = 0; i < size; ++i)
			json_array_append_number(json_array, values[i]);

		return true;
	}
	return false;
}

bool Config::SetFloatArray(const char * name, const float* values, int size)
{
	if (values != nullptr && size > 0)
	{
		JSON_Value* new_array = json_value_init_array();
		json_array = json_value_get_array(new_array);
		json_object_set_value(root, name, new_array);

		for (int i = 0; i < size; ++i)
			json_array_append_number(json_array, values[i]);

		return true;
	}

	return false;
}

bool Config::SetFloat3(const char * name, const float3 & value)
{
	float new_float3[3] = { value.x, value.y, value.z };
	return SetFloatArray(name, new_float3, 3);
}


//---------------- SAVE ----------------

bool Config::Save()
{
	bool ret = false;

	char* buffer;
	size_t size = GetSize(&buffer);

	//TODO CRETE NEW NAME Using Name, ASSETS_SCENE, ".json"
	std::string output;
	ret = App->fs->Save(output, buffer,size, ASSETS_SCENES_FOLDER, "patata", "json");

	RELEASE_ARRAY(buffer);

	return ret;
}

size_t Config::GetSize(char ** buffer) const
{
	size_t ret = json_serialization_size(json_root);
	*buffer = new char[ret];
	json_serialize_to_buffer(json_root, *buffer, ret);

	return ret;
}

