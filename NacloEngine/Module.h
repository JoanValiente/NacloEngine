#ifndef __Module_H__
#define __Module_H__

#include "Globals.h"

class Application;
struct Config;

class Module
{
private :
	bool enabled;

public:
	Application* App;

	Module(Application* parent, bool start_enabled = true) : App(parent)
	{}

	virtual ~Module()
	{}

	virtual bool Init(Config* conf = nullptr)
	{
		return true; 
	}

	virtual bool Start(Config* conf = nullptr)
	{
		return true;
	}

	virtual update_status PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual bool GameMode() { return true; };

	virtual bool EditorMode() { return true; };

public:
	bool active = true;
};

#endif