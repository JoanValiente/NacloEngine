#ifndef __CONFIGURADION_H__
#define __CONFIGURADION_H__

#include "Globals.h"
#include "MathGeoLib/MathGeoLib.h"

struct json_object_t;
typedef struct json_object_t JSON_Object;

struct json_value_t;
typedef struct json_value_t  JSON_Value;

struct json_array_t;
typedef struct json_array_t  JSON_Array;

class Config
{
public:
	Config();
	Config(const char* data);
	Config(JSON_Object* section);
	~Config();

	Config GetSection(const char* section_name) const;
	Config AddSection(const char* section_name);

	bool GetBool(const char * file, bool value = false) const;
	bool AddBool(const char* file, bool value);

	int GetInt(const char* file, int value = 0) const;
	bool AddInt(const char* file, int value);

	UUID GetUUID(const char* file, UUID value = 0) const;
	bool AddUUID(const char* file, UUID value);

	float GetFloat(const char* file, float value = 0.f) const;
	bool AddFloat(const char* file, float value);

	const char* GetString(const char* file, const char* value = nullptr) const;
	bool AddString(const char* file, const char* string);


	Config GetArray(const char* file, int index) const;
	bool AddArray(const char* array_name);
	bool AddArrayEntry(const Config& config);

private:
	JSON_Value * FindValue(const char* field, int index) const;

private:
	JSON_Value * json_root = nullptr;
	JSON_Object* root = nullptr;
	JSON_Array* array = nullptr;
	int index = -1;
};

#endif // __CONFIGURADION_H__
