#ifndef __MODULETIMER_H__
#define __MODULETIMER_H__

#ifndef GAME_MODE
#include "Module.h"

class ModuleTimer : public Module
{
public:

	ModuleTimer(Application* app, bool start_enabled = true);
	virtual ~ModuleTimer();

	update_status PreUpdate();
	
	uint GetFrameCount() const;
	float GetTime() const;
	float GetTimeScale() const;
	float Getdt() const;
	float GetRealTime() const;
	float GetRealdt() const;

private:
	uint frameCount = 0; //app graphics frames since game start
	float time = 0.0f; //seconds since game start (game clock)
	float timeScale = 0.0f; //scale at which time is passing (game clock)
	float deltaTime = 0.0f; //last frame time expressed in seconds (game clock)
	float realTimeSinceStartup = 0.0f; //seconds since game start (real time clock)
	float realTimeDeltaTime = 0.0f; //last frame time expressed in seconds (real time clock)
};

#endif // __MODULETIMER_H__

#endif