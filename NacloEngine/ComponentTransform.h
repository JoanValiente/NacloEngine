#ifndef __COMPONENTTRANSFORM_H__
#define __COMPONENTTRANSFORM_H__

#include "Component.h"
#include "MathGeoLib/MathGeoLib.h"

struct GameObject;

class ComponentTransform : public Component{

public:
	ComponentTransform(GameObject* parent, float3 position, float3 rotation, float3 size);
	~ComponentTransform();

	void UpdateMatrix(float3 position, Quat quaternion, float3 size);

	void SetPosition(float3 position);
	void SetRotation(float3 rotation);
	void SetSize(float3 size);
	void SetQuaternion(Quat quaternion);

public:
	float3 position = { 0,0,0 };
	float3 rotation = { 0,0,0 };
	float3 size = { 1,1,1 };
	Quat quaternion = { 0,0,0,0 };
	float4x4 matrix;
};

#endif
