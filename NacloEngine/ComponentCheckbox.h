#ifndef _ComponentCheckbox_H_
#define _ComponentCheckbox_H_

#include "ComponentInteractive.h"
#include "MathGeoLib/MathGeoLib.h"

struct Image;
struct GameObject;
struct Config;

class ComponentCheckbox : public ComponentInteractive
{
public:
	ComponentCheckbox(GameObject* container);
	~ComponentCheckbox();

	void ShowInspector();

	void Update(float dt);

	void DebugDraw();

	void SaveComponent(Config &conf);
	void LoadComponent(Config &conf);

	void Hover();
	void Enter();
	void Exit();
	void OnClick();

	void ExecuteFunction();

private:
	float2 size = { 10, 5 };
	float3 pos = float3::zero;
	bool checked = false;
};

#endif
