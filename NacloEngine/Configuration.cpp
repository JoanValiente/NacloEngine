#include "Configuration.h"
#include "parson.h"

Config::Config()
{
}

Config::Config(const char * data)
{
	if (data != nullptr)
	{
		json_root = json_parse_string(data);
		if (json_root != nullptr) {
			root = json_value_get_object(json_root);
		}
	}
}

Config::Config(JSON_Object * section)
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

bool Config::GetBool(const char * file, bool value) const
{
	JSON_Value* ret = FindValue(file, index);

	if (ret && json_value_get_type(ret) == JSONBoolean)
		return json_value_get_boolean(ret) != 0;

	return value;
}

bool Config::AddBool(const char * file, bool value)
{
	return false;
}

int Config::GetInt(const char * file, int value) const
{
	JSON_Value* ret = FindValue(file, index);

	if (ret && json_value_get_type(ret) == JSONNumber)
		return (int)json_value_get_number(ret);

	return value;
}

bool Config::AddInt(const char * file, int value)
{
	return false;
}

UUID Config::GetUUID(const char * file, UUID value) const
{
	JSON_Value* ret = FindValue(file, index);

	if (ret && json_value_get_type(ret) == JSONNumber)
		return (UUID)json_value_get_number(ret);

	return value;
}

bool Config::AddUUID(const char * file, UUID value)
{
	return false;
}

float Config::GetFloat(const char * file, float value) const
{
	JSON_Value* ret = FindValue(file, index);

	if (ret && json_value_get_type(ret) == JSONNumber)
		return (float)json_value_get_number(ret);

	return value;
}

bool Config::AddFloat(const char * file, float value)
{
	return false;
}

const char * Config::GetString(const char * file, const char * value) const
{
	JSON_Value* ret = FindValue(file, index);

	if (ret && json_value_get_type(ret) == JSONString)
		return json_value_get_string(ret);

	return value;
}

bool Config::AddString(const char * file, const char * string)
{
	return false;
}

Config Config::GetArray(const char * file, int index) const
{
	JSON_Array* array = json_object_get_array(root, file);

	if (array != nullptr)
		return Config(json_array_get_object(array, index));

	return Config((JSON_Object*) nullptr);
}

bool Config::AddArray(const char * array_name)
{
	return false;
}

bool Config::AddArrayEntry(const Config & config)
{
	return false;
}

JSON_Value * Config::FindValue(const char * file, int index) const
{
	return nullptr;
}
