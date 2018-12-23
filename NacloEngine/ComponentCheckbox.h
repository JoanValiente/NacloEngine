#ifndef _ComponentCheckbox_H_
#define _ComponentCheckbox_H_

#include "ComponentInteractive.h"
#include "MathGeoLib/MathGeoLib.h"

struct ComponentImage;
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
	void FloatToImVec(float4 aux, ImVec4 & vec);
	void OpenPopUps();

private:
	float2 size = { 10, 10 };
	ComponentImage* target_graphic = nullptr;
	ComponentImage* graphic = nullptr;

	ImVec4 normal_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 highlighted_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 pressed_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

	bool isOn = false;
};

#endif
