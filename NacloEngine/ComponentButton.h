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

	void isHovered();

private:
	void SetValueOneToZero(float &x, float &y);
	void SetMouseValueOneToZero(float &x, float &y);

private: 
	float2 size = {10, 5};
	float3 pos = float3::zero;
};

#endif
