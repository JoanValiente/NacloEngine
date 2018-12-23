#ifndef _ComponentInteractive_H_
#define _ComponentInteractive_H_

#include "MathGeoLib/MathGeoLib.h"

struct ComponentCanvas;

enum InteractiveStates
{
	NO_STATE,
	ENTER,
	HOVER,
	EXIT
};

class ComponentInteractive : public Component
{
public:
	ComponentInteractive(GameObject* container);
	~ComponentInteractive();
	
	void UpdateInteractive();

	void AddToTheList();


	virtual void Enter() {};
	virtual void Hover() {};
	virtual void Down() {};
	virtual void OnClick() {};
	virtual void Exit() {};
	virtual void Idle() {};
	void Move();

	virtual void SaveComponent(Config &conf);
	void LoadComponent(Config &conf);


	ComponentCanvas* GetCanvas();
private:
	void SetValueOneToZero(float &x, float &y);
	void SetMouseValueOneToZero(float &x, float &y);

public:
	bool added = false;
	bool dragable = false;
	InteractiveStates state = NO_STATE;
	ComponentCanvas* incanvas = nullptr;
};

#endif
