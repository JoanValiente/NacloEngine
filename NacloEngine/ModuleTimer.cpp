#include "ModuleTimer.h"
#include "Application.h"

ModuleTimer::ModuleTimer(Application * app, bool start_enabled) : Module (app, start_enabled)
{
}

ModuleTimer::~ModuleTimer()
{
}

update_status ModuleTimer::PreUpdate()
{
	frameCount++;
	realTimeDeltaTime = App->GetDt();
	realTimeSinceStartup += realTimeDeltaTime;

	switch (App->engineState)
	{
	case ENGINE_STATE::PLAY:
		time += realTimeDeltaTime;
		deltaTime = realTimeDeltaTime * timeScale;
		break;
	case ENGINE_STATE::EDITOR:
		time = 0.0f;
		break;
	case ENGINE_STATE::PAUSE:
		deltaTime = 0.0f;
		break;
	default:
		break;
	}

	return UPDATE_CONTINUE;
}

uint ModuleTimer::GetFrameCount() const
{
	return frameCount;
}

float ModuleTimer::GetTime() const
{
	return time;
}

float ModuleTimer::GetTimeScale() const
{
	return timeScale;
}

float ModuleTimer::Getdt() const
{
	return deltaTime;
}

float ModuleTimer::GetRealTime() const
{
	return realTimeSinceStartup;
}

float ModuleTimer::GetRealdt() const
{
	return realTimeDeltaTime;
}

