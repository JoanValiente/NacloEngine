#ifndef _ComponentButton_H_
#define _ComponentButton_H_

#include "MathGeoLib/MathGeoLib.h"

class ComponentButton : public Component
{
public:
	ComponentButton(GameObject* container);

	void Update(float dt);

	void ShowInspector();

	void DebugDraw();

	void SaveComponent(Config &conf);
	void LoadComponent(Config &conf);

	float2 SearchCanvas(GameObject* go);

	void isHovered();

private: 
	float2 size = {10, 5};
	float3 pos = float3::zero;
	float2 canvas_size = float2::zero;
};

#endif
