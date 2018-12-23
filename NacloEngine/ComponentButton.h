#ifndef _ComponentButton_H_
#define _ComponentButton_H_

#include "ComponentInteractive.h"
#include "MathGeoLib/MathGeoLib.h"

struct ComponentImage;

class ComponentButton : public ComponentInteractive
{
public:
	ComponentButton(GameObject* container);
	~ComponentButton();


	void Update(float dt);

	void ShowInspector();

	void DebugDraw();

	void SaveComponent(Config &conf);
	void LoadComponent(Config &conf);

	void Idle();
	void Enter();
	void Hover();
	void Down();
	void OnClick();
	void Exit();



private:
	void FloatToImVec(float4 aux, ImVec4 &vec);
	void OpenPopUps();
private: 
	ImVec4 normal_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 highlighted_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 pressed_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ComponentImage * target_graphic;
	float2 size = {20, 10};
};

#endif
