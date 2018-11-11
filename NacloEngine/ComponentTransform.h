#ifndef __COMPONENTTRANSFORM_H__
#define __COMPONENTTRANSFORM_H__

#include "Component.h"
#include "MathGeoLib/MathGeoLib.h"

class ComponentTransform : public Component{

public:
	ComponentTransform(GameObject* container);
	~ComponentTransform();

	void UpdateMatrix(float3 position, Quat quaternion, float3 size);
	void ShowInspector();

	void SetPosition(float3 position);
	void SetRotation(float3 rotation);
	void SetSize(float3 size);
	void SetQuaternion(Quat quaternion);

	float4x4 GetLocalMatrix();
	float4x4 GetGlobalMatrix();

	void SetGlobalMatrix(float4x4 matrix);

public:
	float3 position = float3::zero;
	float3 rotation = float3::zero;
	float3 size = { 1,1,1 };
	Quat quaternion = Quat::identity;
	float4x4 localmatrix = float4x4::identity;
	float4x4 globalMatrix = float4x4::identity;
};

#endif
