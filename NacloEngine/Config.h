#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "Parson\parson.h"
#include "Globals.h"

struct json_object_t;
typedef struct json_object_t JSON_Object;

struct json_value_t;
typedef struct json_value_t  JSON_Value;

class Config
{
public:
	Config();
	Config(const char* data);
	~Config();

	bool GetBool(const char * name) const;
	bool AddBool(const char* name, bool value = false);

	int GetInt(const char* name) const;
	bool SetInt(const char* name, int value = 0);

	int GetFloat(const char* name) const;
	bool SetFloat(const char* name, int value = 0);

	size_t Save(char** buf) const;

public:
	const char* name;

private:
	JSON_Value* json_root = nullptr;
	JSON_Object* root = nullptr;

};

#endif // __CONFIG_H__
