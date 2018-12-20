#ifndef _ComponentButton_H_
#define _ComponentButton_H_

#include "ComponentInteractive.h"
#include "MathGeoLib/MathGeoLib.h"

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


private: 
	float2 size = {10, 5};
	float3 pos = float3::zero;
};

#endif
