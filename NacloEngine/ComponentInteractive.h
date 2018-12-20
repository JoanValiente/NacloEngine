#ifndef _ComponentInteractive_H_
#define _ComponentInteractive_H_

#include "MathGeoLib/MathGeoLib.h"

struct ComponentCanvas;

enum InteractiveStates
{
	NO_STATE,
	ENTER,
	HOVER,
	DOWN,
	EXIT
};

class ComponentInteractive : public Component
{
public:
	ComponentInteractive(GameObject* container);
	~ComponentInteractive();

	void Update(float dt);

	void UpdateInteractive();

	void isHovered();


	void Enter();
	void Hover();
	void Down();
	void Exit();

	void SaveComponent(Config &conf);
	void LoadComponent(Config &conf);

private:
	void SetValueOneToZero(float &x, float &y);
	void SetMouseValueOneToZero(float &x, float &y);

private:
	ComponentCanvas* canvas = nullptr;
};

#endif
