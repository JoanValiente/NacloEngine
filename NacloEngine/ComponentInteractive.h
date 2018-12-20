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

	void UpdateInteractive();

	void isHovered();


	void Enter();
	void Hover();
	void Down();
	void Exit();

	void SaveComponent(Config &conf);
	void LoadComponent(Config &conf);


	ComponentCanvas* GetCanvas();
private:
	void SetValueOneToZero(float &x, float &y);
	void SetMouseValueOneToZero(float &x, float &y);

public:
	ComponentCanvas* canvas = nullptr;
};

#endif
