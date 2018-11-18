#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "Globals.h"
#include "MathGeoLib/MathGeoLib.h"
#include "Parson\parson.h"

class Config
{
public:
	Config();
	Config(const char* data);
	Config(JSON_Object* section);
	~Config();

	Config GetSection(const char * section_name) const;
	Config AddSection(const char * section_name);

	bool GetBool(const char * name) const;
	bool SetBool(const char* name, bool value = false);

	int GetInt(const char* name) const;
	bool SetInt(const char* name, int value = 0);

	float GetFloat(const char* name) const;
	bool SetFloat(const char* name, float value = 0);

	const char* GetString(const char* name) const;
	bool SetString(const char* name, const char* value = "");

	UID GetUID(const char* name) const;
	bool SetUID(const char* name, UID value = 0);

	bool SetArray(const char* name);
	bool NewArrayEntry(const Config &conf);
	Config GetArray(const char* name, int index = -1) const;
	size_t GetArraySize(const char* name) const;


	bool SetIntArray(const char* name, int* values, int size);
	bool SetFloatArray(const char* name, const float* values, int size);

	bool SetFloat3(const char* name, const float3 value);
	float3 GetFloat3(const char* name);


	bool Save();

private:
	std::string GenerateSceneName(const char* folder, const char* file_name);
	size_t GetSize(char** buf) const;

private:
	const char* name;
	JSON_Array*		json_array	= nullptr;
	JSON_Value*		json_root	= nullptr;
	JSON_Object*	root		= nullptr;

};

#endif // __CONFIG_H__
