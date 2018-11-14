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

	size_t Save(char** buf) const;

private:
	JSON_Value* json_root = nullptr;
	JSON_Object* root = nullptr;

};

#endif // __CONFIG_H__
