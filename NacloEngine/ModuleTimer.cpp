#include "ModuleTimer.h"
#include "Application.h"

#include "mmgr/mmgr.h"

ModuleTimer::ModuleTimer(Application * app, bool start_enabled) : Module (app, start_enabled)
{
	name = "Timer";
}

ModuleTimer::~ModuleTimer()
{
}

void ModuleTimer::EndUpdate()
{
	frameCount++;
	realTimeDeltaTime = App->GetDt();
	realTimeSinceStartup += realTimeDeltaTime;

	if (App->engineState == ENGINE_STATE::EDITOR) {
		time = 0.0f;
	}
	else {
		switch (App->gameState)
		{
		case GAME_STATE::NONE:
			time += realTimeDeltaTime * timeScale;
			deltaTime = realTimeDeltaTime * timeScale;
			break;
		case GAME_STATE::PAUSE:
			deltaTime = 0.0f;
			break;
		case GAME_STATE::TICK:
			deltaTime = 0.0f;
			if (isTick) {
				App->gameState = GAME_STATE::NONE;
				isTick = false;
			}
			break;
		default:
			break;
		}
	}
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

