#ifndef __MODULEFILES_H__
#define __MODULEFILES_H__

#include "Module.h"

class ModuleFiles : public Module
{
public:

	ModuleFiles(Application* app, bool start_enabled = true);
	virtual ~ModuleFiles();

	bool Start();
	bool CleanUp();

	bool LoadFBX();


private:

};

#endif // __ModuleAudio_H__